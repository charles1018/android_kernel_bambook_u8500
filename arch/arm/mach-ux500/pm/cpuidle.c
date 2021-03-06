/*
 * Copyright (C) STMicroelectronics 2009
 * Copyright (C) ST-Ericsson SA 2010-2012
 *
 * Author: Rickard Andersson <rickard.andersson@stericsson.com>,
 *	   Jonas Aaberg <jonas.aberg@stericsson.com> for ST-Ericsson.
 *
 * Loosely based on cpuidle.c by Sundar Iyer.
 *
 * License terms: GNU General Public License (GPL) version 2
 *
 */
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/tick.h>
#include <linux/clockchips.h>
#include <linux/mfd/dbx500-prcmu.h>
#include <linux/regulator/db8500-prcmu.h>

#include <mach/pm.h>
#include <mach/pm-timer.h>
#include <mach/context.h>

#include "cpuidle.h"
#include "cpuidle_dbg.h"

#define MAX_STATE_DETERMINE_LOOP_TIME 100000 /* usec */

struct cpu_state {
	int gov_cstate;
	ktime_t sched_wake_up;
	struct cpuidle_device dev;
	bool restore_arm_core;
	bool restore_arm_ret;
	struct cpuidle_driver *driver;
	struct cpuidle_device device;
};

static DEFINE_PER_CPU(struct cpu_state, *cpu_state);

static DEFINE_SPINLOCK(cpuidle_lock);
static bool restore_ape; /* protected by cpuidle_lock */
static bool restore_arm; /* protected by cpuidle_lock */
static ktime_t time_next;  /* protected by cpuidle_lock */

static atomic_t idle_cpus_counter = ATOMIC_INIT(0);
static atomic_t master_counter = ATOMIC_INIT(0);

static struct dbx500_cpuidle_platform_data *settings;

static void restore_sequence(struct cpu_state *state)
{
	spin_lock(&cpuidle_lock);

	smp_rmb();
	if (state->restore_arm_core) {
		state->restore_arm_core = false;
		smp_wmb();

		context_restore_cpu_registers();
		context_varm_restore_core();
	}

	smp_rmb();
	if (restore_arm) {

		restore_arm = false;
		smp_wmb();

		/* Restore gic settings */
		context_varm_restore_common();
	}

	/* Restore PPI irqs after in ARM retention */
	if (state->restore_arm_ret) {
		state->restore_arm_ret = false;
		context_gic_dist_restore_ppi_irqs();
	}

	smp_rmb();
	if (restore_ape) {
		ktime_t now;

		restore_ape = false;
		smp_wmb();

		/*
		 * APE has been turned off. Save GPIO wake up cause before
		 * clearing ioforce.
		 */
		context_vape_restore();

		ux500_pm_gpio_save_wake_up_status();

		/* Restore IO ring */
		ux500_pm_prcmu_set_ioforce(false);

		ux500_ci_dbg_console_handle_ape_resume();

		ux500_rtcrtt_off();

		/*
		 * Restore prcmu interrupt to cpu0
		 */
		irq_set_affinity(settings->prcmu_interrupt, cpumask_of(0));

		/*
		 * If we're returning from ApSleep and the RTC timer
		 * caused the wake up, program the MTU (in db8500) to trigger.
		 */
		now = ktime_get();

		if ((ktime_to_us(now) >= ktime_to_us(time_next)))
			time_next = ktime_add(now, ktime_set(0, 1000));
		/* Make sure have an MTU interrupt waiting for us */
		if (settings->broadcast_clkevt)
			WARN_ON(clockevents_program_event(settings->broadcast_clkevt,
							  time_next,
							  true));
	}

	spin_unlock(&cpuidle_lock);

}

/**
 * get_remaining_sleep_time() - returns remaining sleep time in
 * microseconds (us)
 */
static u32 get_remaining_sleep_time(ktime_t *next, int *on_cpu)
{
	ktime_t now, t;
	int cpu;
	int delta;
	u32 remaining_sleep_time = UINT_MAX;

	now = ktime_get();

	/* Check next schedule to expire considering both cpus */

	spin_lock(&cpuidle_lock);
	for_each_online_cpu(cpu) {
		t = per_cpu(cpu_state, cpu)->sched_wake_up;

		delta = ktime_to_us(ktime_sub(t, now));

		if (delta < remaining_sleep_time) {
			if (delta > 0)
				remaining_sleep_time = (u32)delta;
			else
				remaining_sleep_time = 0;

			if (next)
				(*next) = t;
			if (on_cpu)
				(*on_cpu) = cpu;
		}
	}
	spin_unlock(&cpuidle_lock);

	return remaining_sleep_time;
}

static bool is_last_cpu_running(void)
{
	smp_rmb();
	return atomic_read(&idle_cpus_counter) == num_online_cpus();
}

static int determine_sleep_state(u32 *sleep_time, int loc_idle_counter,
				 bool gic_frozen, ktime_t entry_time,
				 ktime_t *est_wake_time)
{
	int i;

	int cpu;
	int max_depth;
	bool uart, modem, ape;
	s64 delta_us;

	/* If first cpu to sleep, go to most shallow sleep state */
	if (loc_idle_counter != num_online_cpus())
		return CI_WFI;

	/*
	 * This loop continuously checks if there is an IRQ and exits
	 * immediately if there is, so we shouldn't count the time spent
	 * in there as "irqs off" time.
	 */
	stop_critical_timings();

	/* If other CPU is going to WFI, but not yet there wait. */
	while (1) {
		if (ux500_pm_other_cpu_wfi())
			break;

		/* Check for pending IRQ's */
		if (ux500_pm_gic_pending_interrupt(NULL)) {
			start_critical_timings();
			return -1;
		}

		/* If GIC frozen check for pending IRQ's also via PRCMU */
		if (gic_frozen && ux500_pm_prcmu_pending_interrupt(NULL)) {
			start_critical_timings();
			return -1;
		}

		if (!is_last_cpu_running()) {
			start_critical_timings();
			return CI_WFI;
		}

		delta_us = ktime_us_delta(ktime_get(), entry_time);
		if (delta_us > MAX_STATE_DETERMINE_LOOP_TIME) {
			start_critical_timings();
			pr_warning("%s: CPU=%d stuck in loop for %lld usec\n",
				__func__, smp_processor_id(), delta_us);
			return -1;
		}
	}

	start_critical_timings();

	(*sleep_time) = get_remaining_sleep_time(est_wake_time, NULL);

	if (((*sleep_time) == UINT_MAX) || ((*sleep_time) == 0))
		return CI_WFI;
	/*
	 * Never go deeper than the governor recommends even though it might be
	 * possible from a scheduled wake up point of view
	 */
	max_depth = ux500_ci_dbg_deepest_state();

	for_each_online_cpu(cpu) {
		if (max_depth > per_cpu(cpu_state, cpu)->gov_cstate)
			max_depth = per_cpu(cpu_state, cpu)->gov_cstate;
	}

	uart = ux500_ci_dbg_force_ape_on();
	ape = power_state_active_is_enabled();
	modem = prcmu_is_ac_wake_requested();

	for (i = max_depth; i > 0; i--) {

		if ((*sleep_time) <= settings->cstates[i].threshold)
			continue;

		if (settings->cstates[i].APE == APE_OFF) {
			/* This state says APE should be off */
			if (ape || modem || uart)
				continue;
		}

		/* OK state */
		break;
	}

	ux500_ci_dbg_register_reason(i, ape, modem, uart,
				     (*sleep_time),
				     max_depth);
	return max(CI_WFI, i);
}

static int enter_sleep(struct cpuidle_device *dev,
		       struct cpuidle_driver *drv,
		       int state_num)
{
	ktime_t time_enter, time_exit, time_wake;
	ktime_t wake_up;
	int sleep_time = 0;
	s64 diff;
	int rtcrtt_program_time = NO_SLEEP_PROGRAMMED;
	int target;
	struct cpu_state *state;
	bool slept_well = false;
	int this_cpu = smp_processor_id();
	bool migrate_timer;
	bool master = false;
	int loc_idle_counter;
	ktime_t est_wake_time;

	local_irq_disable();

	time_enter = ktime_get(); /* Time now */

	state = per_cpu(cpu_state, smp_processor_id());

	est_wake_time = wake_up = ktime_add(time_enter,
					    tick_nohz_get_sleep_length());

	spin_lock(&cpuidle_lock);

	/* Save scheduled wake up for this cpu */
	state->sched_wake_up = wake_up;

	/* Retrive the cstate that the governor recommends for this CPU */
	state->gov_cstate = state_num;

	if (state->gov_cstate > ux500_ci_dbg_deepest_state())
		state->gov_cstate = ux500_ci_dbg_deepest_state();

	if (settings->cstates[state->gov_cstate].ARM != ARM_ON)
		migrate_timer = true;
	else
		migrate_timer = false;

	spin_unlock(&cpuidle_lock);

	loc_idle_counter = atomic_inc_return(&idle_cpus_counter);

	/*
	 * Determine sleep state considering both CPUs and
	 * shared resources like e.g. VAPE
	 */
	target = determine_sleep_state(&sleep_time, loc_idle_counter, false,
				       time_enter, &est_wake_time);

	if (target < 0)
		/* "target" will be last_state in the cpuidle framework */
		goto exit_fast;

	/* Only one CPU should master the sleeping sequence */
	if (settings->cstates[target].ARM != ARM_ON) {
		smp_mb();
		if (atomic_inc_return(&master_counter) == 1)
			master = true;
		else
			atomic_dec(&master_counter);
		smp_mb();
	}

	if (migrate_timer)
		clockevents_notify(CLOCK_EVT_NOTIFY_BROADCAST_ENTER,
				   &this_cpu);

	if (master && (settings->cstates[target].ARM != ARM_ON)) {
		ux500_pm_gic_decouple();

		/* Copy GIC interrupt settings to PRCMU interrupt settings */
		ux500_pm_prcmu_copy_gic_settings();

		smp_rmb();
		loc_idle_counter = atomic_read(&idle_cpus_counter);

		/*
		 * Check if sleep state has changed after GIC has been frozen
		 */
		if (target != determine_sleep_state(&sleep_time,
						    loc_idle_counter,
						    true,
						    time_enter,
						    &est_wake_time)) {
			atomic_dec(&master_counter);
			goto exit;
		}

		if (ux500_pm_gic_pending_interrupt(NULL)) {
			/* An interrupt found => abort */
			atomic_dec(&master_counter);
			goto exit;
		}

		if (ux500_pm_prcmu_pending_interrupt(NULL)) {
			/* An interrupt found => abort */
			atomic_dec(&master_counter);
			goto exit;

		}
		/*
		 * No PRCMU interrupt was pending => continue the
		 * sleeping stages
		 */
	}

	if (master && (settings->cstates[target].APE == APE_OFF)) {
		int wake_cpu;

		/* We are going to sleep or deep sleep => prepare for it */

		/* Program the only timer that is available when APE is off */

		sleep_time = get_remaining_sleep_time(&est_wake_time,
						      &wake_cpu);

		if ((sleep_time == UINT_MAX) || (sleep_time == 0)) {
			atomic_dec(&master_counter);
			goto exit;
		}

		if (settings->cstates[target].UL_PLL == UL_PLL_OFF)
			/* Compensate for ULPLL start up time */
			sleep_time -= settings->ul_pll_start_up_latency;

		/*
		 * Not checking for negative sleep time since
		 * determine_sleep_state has already checked that
		 * there is enough time.
		 */

		/* Adjust for exit latency */
		sleep_time -= settings->min_sleep_wake_up_latency;

		ux500_rtcrtt_next(sleep_time);
		rtcrtt_program_time = sleep_time;

		/*
		 * Make sure the cpu that is scheduled first gets
		 * the prcmu interrupt.
		 */
		irq_set_affinity(settings->prcmu_interrupt, cpumask_of(wake_cpu));

		context_vape_save();

		ux500_ci_dbg_console_handle_ape_suspend();
		ux500_pm_prcmu_set_ioforce(true);

		spin_lock(&cpuidle_lock);
		restore_ape = true;
		time_next = est_wake_time;
		spin_unlock(&cpuidle_lock);
	}

	/* Store and disable PPI irqs in ARM retention */
	if (settings->cstates[state->gov_cstate].ARM == ARM_RET) {
		context_gic_dist_store_ppi_irqs();
		state->restore_arm_ret = true;
	}

	if (master && (settings->cstates[target].ARM == ARM_OFF)) {
		int cpu;

		context_varm_save_common();

		spin_lock(&cpuidle_lock);
		restore_arm = true;
		for_each_possible_cpu(cpu)
			(per_cpu(cpu_state, cpu))->restore_arm_core = true;

		spin_unlock(&cpuidle_lock);
	}

	if (settings->cstates[state->gov_cstate].ARM == ARM_OFF) {
		context_varm_save_core();

		if (master && (settings->cstates[target].ARM == ARM_OFF))
			context_gic_dist_disable_unneeded_irqs();

		context_save_cpu_registers();

		/*
		 * Due to we have only 100us between requesting a
		 * powerstate and wfi, we clean the cache before as
		 * well to assure the final cache clean before wfi
		 * has as little as possible to do.
		 */
		context_clean_l1_cache_all();
	}

	ux500_ci_dbg_log(target, time_enter);

	ux500_ci_dbg_log_post_mortem(target,
				     state->gov_cstate,
				     time_enter,
				     est_wake_time,
				     state->sched_wake_up,
				     rtcrtt_program_time,
				     is_last_cpu_running());

	if (master && settings->cstates[target].ARM != ARM_ON)
		prcmu_set_power_state(settings->cstates[target].pwrst,
				      settings->cstates[target].UL_PLL,
				      /* Is actually the AP PLL */
				      settings->cstates[target].UL_PLL);

	if (master)
		atomic_dec(&master_counter);

	stop_critical_timings();

	/*
	 * If deepsleep/deepidle, Save return address to SRAM and set
	 * this CPU in WFI. This is last core to enter sleep, so we need to
	 * clean both L2 and L1 caches
	 */
	if (settings->cstates[state->gov_cstate].ARM == ARM_OFF)
		context_save_to_sram_and_wfi(settings->cstates[target].ARM == ARM_OFF);
	else
		__asm__ __volatile__
			("dsb\n\t" "wfi\n\t" : : : "memory");

	start_critical_timings();

	if (is_last_cpu_running())
		ux500_ci_dbg_wake_latency(target, sleep_time);

	time_wake = ktime_get();

	ux500_ci_dbg_wake_time(time_wake);

	slept_well = true;

	restore_sequence(state);

exit:
	if (!slept_well)
		/* Recouple GIC with the interrupt bus */
		ux500_pm_gic_recouple();

	/* Use the ARM local timer for this cpu */
	if (migrate_timer)
		clockevents_notify(CLOCK_EVT_NOTIFY_BROADCAST_EXIT,
				   &this_cpu);
exit_fast:

	atomic_dec(&idle_cpus_counter);

	if (target < 0)
		target = CI_RUNNING;

	ux500_ci_dbg_detect_wake_late(state->sched_wake_up, target);

	/* 16 minutes ahead */
	wake_up = ktime_add_us(time_enter,
			       1000000000);

	spin_lock(&cpuidle_lock);
	/* Remove wake up time i.e. set wake up far ahead */
	state->sched_wake_up = wake_up;
	spin_unlock(&cpuidle_lock);

	time_exit = ktime_get();
	diff = ktime_to_us(ktime_sub(time_exit, time_enter));
	if (diff > INT_MAX)
		diff = INT_MAX;

	dev->last_residency = (int)diff;

	ux500_ci_dbg_console_check_uart();
	if (slept_well)
		ux500_ci_dbg_exit_latency(target,
					  time_exit, /* now */
					  time_wake, /* exit from wfi */
					  time_enter); /* enter cpuidle */

	ux500_ci_dbg_log(CI_RUNNING, time_exit);

	local_irq_enable();

	ux500_ci_dbg_console();

	return target;
}

static void setup_broadcast_timer(void *arg)
{
	unsigned long reason = (unsigned long)arg;
	int cpu = smp_processor_id();

	clockevents_notify(reason, &cpu);
}

static void __init init_cstates(struct cpuidle_driver *driver)
{
	int i;
	struct cpuidle_state *ci_state;

	driver->state_count = settings->cstates_len;

	for (i = 0; i < driver->state_count; i++) {
		ci_state = &driver->states[i];

		ci_state->exit_latency = settings->cstates[i].exit_latency;
		ci_state->target_residency = settings->cstates[i].threshold;
		ci_state->flags = settings->cstates[i].flags;
		ci_state->enter = enter_sleep;
		ci_state->power_usage = settings->cstates[i].power_usage;
		snprintf(ci_state->name, CPUIDLE_NAME_LEN, "C%d", i);
		strncpy(ci_state->desc, settings->cstates[i].desc, CPUIDLE_DESC_LEN);
	}
}

struct cpuidle_driver dbx500_cpuidle_driver = {
	.name = "dbx500-cpuidle",
	.owner = THIS_MODULE,
};

static int __init dbx500_cpuidle_probe(struct platform_device *pdev)
{
	int ret = -EINVAL;
	int cpu;
	int i;

	on_each_cpu(setup_broadcast_timer,
		    (void *)CLOCK_EVT_NOTIFY_BROADCAST_ON, 1);

	if (!pdev->dev.platform_data) {
		dev_err(&pdev->dev, "error: no platform data.\n");
		goto out_nodriver;
	}

	settings = (struct dbx500_cpuidle_platform_data *)pdev->dev.platform_data;

	for_each_possible_cpu(cpu) {
		per_cpu(cpu_state, cpu) = devm_kzalloc(&pdev->dev,
						       sizeof(struct cpu_state),
						       GFP_KERNEL);
		if (!per_cpu(cpu_state, cpu)) {
			ret = -ENOMEM;
			goto out_nodriver;
		}
		per_cpu(cpu_state, cpu)->driver = &dbx500_cpuidle_driver;
	}

	init_cstates(&dbx500_cpuidle_driver);

	ret = cpuidle_register_driver(&dbx500_cpuidle_driver);
	if (ret)
		goto out_nodriver;

	ux500_ci_dbg_init(&pdev->dev, settings->cstates, settings->cstates_len);

	for_each_possible_cpu(cpu) {
		struct cpuidle_device *dev;

		dev = &per_cpu(cpu_state, cpu)->device;
		dev->cpu = cpu;
		dev->state_count = settings->cstates_len;

		ret = cpuidle_register_device(dev);

		if (ret)
			goto out_cstates;
		dev_info(&pdev->dev, "initiated for CPU%d.\n", cpu);
	}

	/* Configure wake up reasons */
	prcmu_enable_wakeups(settings->wakeups);

	return 0;
out_cstates:
	for (i = cpu - 1; i >= 0; i--)
		cpuidle_unregister_device(&per_cpu(cpu_state, i)->dev);

	cpuidle_unregister_driver(&dbx500_cpuidle_driver);

	ux500_ci_dbg_remove();
out_nodriver:
	on_each_cpu(setup_broadcast_timer,
		    (void *)CLOCK_EVT_NOTIFY_BROADCAST_OFF, 1);
	dev_err(&pdev->dev, "initialization failed.\n");
	return ret;
}

static void __exit dbx500_cpuidle_exit(void)
{
	int cpu;

	ux500_ci_dbg_remove();

	for_each_possible_cpu(cpu)
		cpuidle_unregister_device(&per_cpu(cpu_state, cpu)->dev);

	for_each_possible_cpu(cpu)
		kfree(per_cpu(cpu_state, cpu));

	cpuidle_unregister_driver(&dbx500_cpuidle_driver);
}

static struct platform_driver dbx500_cpuidle_plat_driver = {
	.driver = {
		.name = "dbx500-cpuidle",
		.owner = THIS_MODULE,
	},
};

static int __init dbx500_cpuidle_init(void)
{
	return platform_driver_probe(&dbx500_cpuidle_plat_driver,
				     dbx500_cpuidle_probe);
}
late_initcall(dbx500_cpuidle_init);
module_exit(dbx500_cpuidle_exit);

MODULE_DESCRIPTION("U8500 cpuidle driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rickard Andersson <rickard.andersson@stericsson.com>");
