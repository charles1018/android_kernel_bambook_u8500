/*
 * Copyright (C) ST-Ericsson SA 2010
 *
 * License Terms: GNU General Public License, version 2
 * Author: Rabin Vincent <rabin.vincent@stericsson.com> for ST-Ericsson
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/input/matrix_keypad.h>
#include <linux/mfd/stmpe.h>

/* These are at the same addresses in most of STMPE variants */
#define STMPE_REG_KPC_COL		0x60
#define STMPE_REG_KPC_ROW_MSB		0x61
#define STMPE_REG_KPC_ROW_LSB		0x62
#define STMPE_REG_KPC_CTRL_MSB		0x63
#define STMPE_REG_KPC_CTRL_LSB		0x64
#define STMPE_REG_KPC_COMBI_KEY_0	0x65
#define STMPE_REG_KPC_COMBI_KEY_1	0x66
#define STMPE_REG_KPC_COMBI_KEY_2	0x67
#define STMPE_REG_KPC_DATA_BYTE0	0x68
#define STMPE_REG_KPC_DATA_BYTE1	0x69
#define STMPE_REG_KPC_DATA_BYTE2	0x6a
#define STMPE_REG_KPC_DATA_BYTE3	0x6b
#define STMPE_REG_KPC_DATA_BYTE4	0x6c

#define STMPE_KPC_CTRL_LSB_SCAN		(0x1 << 0)
#define STMPE_KPC_CTRL_LSB_DEBOUNCE	(0x7f << 1)
#define STMPE_KPC_CTRL_MSB_SCAN_COUNT	(0xf << 4)

/* STMPE1801 */
#define STMPE1801_REG_KPC_ROW		0x30
#define STMPE1801_REG_KPC_COL_LOW	0x31
#define STMPE1801_REG_KPC_COL_HIGH	0x32
#define STMPE1801_REG_KPC_CTRL_LOW	0x33
#define STMPE1801_REG_KPC_CTRL_MID	0x34
#define STMPE1801_REG_KPC_CTRL_HIGH	0x35
#define STMPE1801_REG_KPC_CMD		0x36
#define STMPE1801_REG_KPC_COMBI_KEY_0	0x37
#define STMPE1801_REG_KPC_COMBI_KEY_1	0x38
#define STMPE1801_REG_KPC_COMBI_KEY_2	0x39
#define STMPE1801_REG_KPC_DATA_BYTE0	0x3a
#define STMPE1801_REG_KPC_DATA_BYTE1	0x3b
#define STMPE1801_REG_KPC_DATA_BYTE2	0x3c
#define STMPE1801_REG_KPC_DATA_BYTE3	0x3d
#define STMPE1801_REG_KPC_DATA_BYTE4	0x3e

#define STMPE1801_MSK_KPC_SCAN_COUNT	(0xf << 4)
#define STMPE1801_MSK_KPC_DEBOUNCE	(0x3f << 2)
#define STMPE1801_MSK_KPC_CMD_SCAN	(0x1 << 0)

#define STMPE_KPC_ROW_MSB_ROWS		0xff

#define STMPE_KPC_DATA_UP		(0x1 << 7)

#define STMPE_KPC_DATA_NOKEY_MASK	0x78

#define STMPE_KEYPAD_MAX_DEBOUNCE	127
#define STMPE_KEYPAD_MAX_SCAN_COUNT	15

#define STMPE_KEYPAD_MAX_ROWS		8
#define STMPE_KEYPAD_MAX_COLS		10

#define STMPE_KEYPAD_KEYMAP_SIZE	\
	(STMPE_KEYPAD_MAX_ROWS * STMPE_KEYPAD_MAX_COLS)

enum {
	STMPE_IDX_KPC_COL_LSB,
	STMPE_IDX_KPC_COL_MSB,
	STMPE_IDX_KPC_ROW_LSB,
	STMPE_IDX_KPC_ROW_MSB,
	STMPE_IDX_KPC_CTRL_LSB,
	STMPE_IDX_KPC_CTRL_MID,
	STMPE_IDX_KPC_CTRL_MSB,
	STMPE_IDX_KPC_CMD,
	STMPE_IDX_KPC_COMBI_KEY_0,
	STMPE_IDX_KPC_COMBI_KEY_1,
	STMPE_IDX_KPC_COMBI_KEY_2,
	STMPE_IDX_KPC_DATA_BYTE0,
	STMPE_IDX_KPC_DATA_BYTE1,
	STMPE_IDX_KPC_DATA_BYTE2,
	STMPE_IDX_KPC_DATA_BYTE3,
	STMPE_IDX_KPC_DATA_BYTE4,
};

static const u8 stmpe_default_regs[] = {
	[STMPE_IDX_KPC_COL_LSB]		= STMPE_REG_KPC_COL,
	[STMPE_IDX_KPC_ROW_LSB]		= STMPE_REG_KPC_ROW_LSB,
	[STMPE_IDX_KPC_ROW_MSB]		= STMPE_REG_KPC_ROW_MSB,
	[STMPE_IDX_KPC_CTRL_LSB]	= STMPE_REG_KPC_CTRL_LSB,
	[STMPE_IDX_KPC_CTRL_MSB]	= STMPE_REG_KPC_CTRL_MSB,
	[STMPE_IDX_KPC_COMBI_KEY_0]	= STMPE_REG_KPC_COMBI_KEY_0,
	[STMPE_IDX_KPC_COMBI_KEY_1]	= STMPE_REG_KPC_COMBI_KEY_1,
	[STMPE_IDX_KPC_COMBI_KEY_2]	= STMPE_REG_KPC_COMBI_KEY_2,
	[STMPE_IDX_KPC_DATA_BYTE0]	= STMPE_REG_KPC_DATA_BYTE0,
	[STMPE_IDX_KPC_DATA_BYTE1]	= STMPE_REG_KPC_DATA_BYTE1,
	[STMPE_IDX_KPC_DATA_BYTE2]	= STMPE_REG_KPC_DATA_BYTE2,
	[STMPE_IDX_KPC_DATA_BYTE3]	= STMPE_REG_KPC_DATA_BYTE3,
	[STMPE_IDX_KPC_DATA_BYTE4]	= STMPE_REG_KPC_DATA_BYTE4,
};

static const u8 stmpe_1801_regs[] = {
	[STMPE_IDX_KPC_COL_LSB]		= STMPE1801_REG_KPC_COL_LOW,
	[STMPE_IDX_KPC_COL_MSB]		= STMPE1801_REG_KPC_COL_HIGH,
	[STMPE_IDX_KPC_ROW_LSB]		= STMPE1801_REG_KPC_ROW,
	[STMPE_IDX_KPC_CTRL_LSB]	= STMPE1801_REG_KPC_CTRL_LOW,
	[STMPE_IDX_KPC_CTRL_MID]	= STMPE1801_REG_KPC_CTRL_MID,
	[STMPE_IDX_KPC_CTRL_MSB]	= STMPE1801_REG_KPC_CTRL_HIGH,
	[STMPE_IDX_KPC_CMD]		= STMPE1801_REG_KPC_CMD,
	[STMPE_IDX_KPC_COMBI_KEY_0]	= STMPE1801_REG_KPC_COMBI_KEY_0,
	[STMPE_IDX_KPC_COMBI_KEY_1]	= STMPE1801_REG_KPC_COMBI_KEY_1,
	[STMPE_IDX_KPC_COMBI_KEY_2]	= STMPE1801_REG_KPC_COMBI_KEY_2,
	[STMPE_IDX_KPC_DATA_BYTE0]	= STMPE1801_REG_KPC_DATA_BYTE0,
	[STMPE_IDX_KPC_DATA_BYTE1]	= STMPE1801_REG_KPC_DATA_BYTE1,
	[STMPE_IDX_KPC_DATA_BYTE2]	= STMPE1801_REG_KPC_DATA_BYTE2,
	[STMPE_IDX_KPC_DATA_BYTE3]	= STMPE1801_REG_KPC_DATA_BYTE3,
	[STMPE_IDX_KPC_DATA_BYTE4]	= STMPE1801_REG_KPC_DATA_BYTE4,
};

/**
 * struct stmpe_keypad_variant - model-specific attributes
 * @auto_increment: whether the KPC_DATA_BYTE register address
 *		    auto-increments on multiple read
 * @num_data: number of data bytes
 * @num_normal_data: number of normal keys' data bytes
 * @max_cols: maximum number of columns supported
 * @max_rows: maximum number of rows supported
 * @row_mask: mask used to get row number in KPC_DATA_BYTEx registers
 * @col_mask: mask used to get column number in KPC_DATA_BYTEx registers
 * @row_shift: shift used to get row number in KPC_DATA_BYTEx registers
 * @col_shift: shift used to get column number in KPC_DATA_BYTEx registers
 * @col_gpios: bitmask of gpios which can be used for columns
 * @row_gpios: bitmask of gpios which can be used for rows
 */
struct stmpe_keypad_variant {
	bool		auto_increment;
	int		num_data;
	int		num_normal_data;
	int		max_cols;
	int		max_rows;
	unsigned int	row_mask;
	unsigned int	col_mask;
	unsigned char	row_shift;
	unsigned char	col_shift;
	unsigned int	col_gpios;
	unsigned int	row_gpios;
	const u8	*regs;
};

static const struct stmpe_keypad_variant stmpe_keypad_variants[] = {
	[STMPE1601] = {
		.auto_increment		= true,
		.num_data		= 5,
		.num_normal_data	= 3,
		.max_cols		= 8,
		.max_rows		= 8,
		.row_mask		= (0xf << 3),
		.row_shift		= 3,
		.col_mask		= (0x7 << 0),
		.col_shift		= 0,
		.col_gpios		= 0x000ff,	/* GPIO 0 - 7 */
		.row_gpios		= 0x0ff00,	/* GPIO 8 - 15 */
		.regs			= stmpe_default_regs,
	},
	[STMPE1801] = {
		.auto_increment		= true,
		.num_data		= 5,
		.num_normal_data	= 3,
		.max_cols		= 10,
		.max_rows		= 8,
		.row_mask		= (0x7 << 0),
		.row_shift		= 0,
		.col_mask		= (0xf << 3),
		.col_shift		= 3,
		.col_gpios		= 0x3ff00,	/* GPIO 8 - 17 */
		.row_gpios		= 0x000ff,	/* GPIO 0 - 7 */
		.regs			= stmpe_1801_regs,
	},
	[STMPE2401] = {
		.auto_increment		= false,
		.num_data		= 3,
		.num_normal_data	= 2,
		.max_cols		= 8,
		.max_rows		= 12,
		.row_mask		= (0xf << 3),
		.row_shift		= 3,
		.col_mask		= (0x7 << 0),
		.col_shift		= 0,
		.col_gpios		= 0x0000ff,	/* GPIO 0 - 7*/
		.row_gpios		= 0x1fef00,	/* GPIO 8-14, 16-20 */
		.regs			= stmpe_default_regs,
	},
	[STMPE2403] = {
		.auto_increment		= true,
		.num_data		= 5,
		.num_normal_data	= 3,
		.max_cols		= 8,
		.max_rows		= 12,
		.row_mask		= (0xf << 3),
		.row_shift		= 3,
		.col_mask		= (0x7 << 0),
		.col_shift		= 0,
		.col_gpios		= 0x0000ff,	/* GPIO 0 - 7*/
		.row_gpios		= 0x1fef00,	/* GPIO 8-14, 16-20 */
		.regs			= stmpe_default_regs,
	},
};

struct stmpe_keypad {
	struct stmpe *stmpe;
	struct input_dev *input;
	const struct stmpe_keypad_variant *variant;
	const struct stmpe_keypad_platform_data *plat;

	unsigned int rows;
	unsigned int cols;
	unsigned char scan_code_row_shift;
	bool enable;

	unsigned short keymap[STMPE_KEYPAD_KEYMAP_SIZE];
};

static ssize_t stmpe_show_attr_enable(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct stmpe_keypad *keypad = platform_get_drvdata(pdev);
	return sprintf(buf, "%d\n", keypad->enable);
}

static ssize_t stmpe_store_attr_enable(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct stmpe_keypad *keypad = platform_get_drvdata(pdev);
	struct stmpe *stmpe = keypad->stmpe;
	unsigned long val;

	if (strict_strtoul(buf, 0, &val))
		return -EINVAL;

	if (keypad->enable != val) {
		keypad->enable = val;
		if (!val)
			stmpe_disable(stmpe, STMPE_BLOCK_KEYPAD);
		else
			stmpe_enable(stmpe, STMPE_BLOCK_KEYPAD);
	}
	return count;
}

static DEVICE_ATTR(enable, S_IWUSR | S_IRUGO,
	stmpe_show_attr_enable, stmpe_store_attr_enable);

static struct attribute *stmpe_keypad_attrs[] = {
	&dev_attr_enable.attr,
	NULL,
};

static struct attribute_group stmpe_attr_group = {
	.attrs = stmpe_keypad_attrs,
};

static int stmpe_keypad_read_data(struct stmpe_keypad *keypad, u8 *data)
{
	const struct stmpe_keypad_variant *variant = keypad->variant;
	struct stmpe *stmpe = keypad->stmpe;
	int ret;
	int i;

	if (variant->auto_increment)
		return stmpe_block_read(stmpe,
				variant->regs[STMPE_IDX_KPC_DATA_BYTE0],
				variant->num_data, data);

	for (i = 0; i < variant->num_data; i++) {
		ret = stmpe_reg_read(stmpe,
				variant->regs[STMPE_IDX_KPC_DATA_BYTE0] + i);
		if (ret < 0)
			return ret;

		data[i] = ret;
	}

	return 0;
}

static irqreturn_t stmpe_keypad_irq(int irq, void *dev)
{
	struct stmpe_keypad *keypad = dev;
	struct input_dev *input = keypad->input;
	const struct stmpe_keypad_variant *variant = keypad->variant;
	u8 fifo[variant->num_data];
	int ret;
	int i;

	ret = stmpe_keypad_read_data(keypad, fifo);
	if (ret < 0)
		return IRQ_NONE;

	for (i = 0; i < variant->num_normal_data; i++) {
		u8 data = fifo[i];
		int row = (data & variant->row_mask) >> variant->row_shift;
		int col = (data & variant->col_mask) >> variant->col_shift;
		int code = MATRIX_SCAN_CODE(row, col,
				keypad->scan_code_row_shift);
		bool up = data & STMPE_KPC_DATA_UP;

		if ((data & STMPE_KPC_DATA_NOKEY_MASK)
			== STMPE_KPC_DATA_NOKEY_MASK)
			continue;

		input_event(input, EV_MSC, MSC_SCAN, code);
		input_report_key(input, keypad->keymap[code], !up);
		input_sync(input);
	}

	return IRQ_HANDLED;
}

static int __devinit stmpe_keypad_altfunc_init(struct stmpe_keypad *keypad)
{
	const struct stmpe_keypad_variant *variant = keypad->variant;
	unsigned int col_gpios = variant->col_gpios;
	unsigned int row_gpios = variant->row_gpios;
	struct stmpe *stmpe = keypad->stmpe;
	unsigned int pins = 0;
	int i;

	/*
	 * Figure out which pins need to be set to the keypad alternate
	 * function.
	 *
	 * {cols,rows}_gpios are bitmasks of which pins on the chip can be used
	 * for the keypad.
	 *
	 * keypad->{cols,rows} are a bitmask of which pins (of the ones useable
	 * for the keypad) are used on the board.
	 */

	for (i = 0; i < variant->max_cols; i++) {
		int num = __ffs(col_gpios);

		if (keypad->cols & (1 << i))
			pins |= 1 << num;

		col_gpios &= ~(1 << num);
	}

	for (i = 0; i < variant->max_rows; i++) {
		int num = __ffs(row_gpios);

		if (keypad->rows & (1 << i))
			pins |= 1 << num;

		row_gpios &= ~(1 << num);
	}

	return stmpe_set_altfunc(stmpe, pins, STMPE_BLOCK_KEYPAD);
}

static int __devinit stmpe_keypad_chip_init(struct stmpe_keypad *keypad)
{
	const struct stmpe_keypad_platform_data *plat = keypad->plat;
	const struct stmpe_keypad_variant *variant = keypad->variant;
	struct stmpe *stmpe = keypad->stmpe;
	int ret;

	if (plat->debounce_ms > STMPE_KEYPAD_MAX_DEBOUNCE)
		return -EINVAL;

	if (plat->scan_count > STMPE_KEYPAD_MAX_SCAN_COUNT)
		return -EINVAL;

	ret = stmpe_enable(stmpe, STMPE_BLOCK_KEYPAD);
	if (ret < 0)
		return ret;

	ret = stmpe_keypad_altfunc_init(keypad);
	if (ret < 0)
		return ret;

	ret = stmpe_reg_write(stmpe, variant->regs[STMPE_IDX_KPC_COL_LSB],
			keypad->cols);
	if (ret < 0)
		return ret;

	if (stmpe->partnum == STMPE1801 && variant->max_cols > 8) {
		ret = stmpe_set_bits(stmpe,
				variant->regs[STMPE_IDX_KPC_COL_MSB],
				0x3,
				keypad->cols >> 8);
		if (ret < 0)
			return ret;
	}

	ret = stmpe_reg_write(stmpe, variant->regs[STMPE_IDX_KPC_ROW_LSB],
			keypad->rows);
	if (ret < 0)
		return ret;

	if (variant->max_rows > 8) {
		ret = stmpe_set_bits(stmpe,
				variant->regs[STMPE_IDX_KPC_ROW_MSB],
				STMPE_KPC_ROW_MSB_ROWS,
				keypad->rows >> 8);
		if (ret < 0)
			return ret;
	}

	if (stmpe->partnum == STMPE1801) {
		ret = stmpe_set_bits(stmpe,
				variant->regs[STMPE_IDX_KPC_CTRL_LSB],
				STMPE1801_MSK_KPC_SCAN_COUNT,
				plat->scan_count << 4);
		if (ret < 0)
			return ret;

		ret = stmpe_set_bits(stmpe,
				variant->regs[STMPE_IDX_KPC_CTRL_MID],
				STMPE1801_MSK_KPC_DEBOUNCE,
				(plat->debounce_ms << 1));
		if (ret < 0)
			return ret;

		return  stmpe_set_bits(stmpe,
				variant->regs[STMPE_IDX_KPC_CMD],
				STMPE1801_MSK_KPC_CMD_SCAN,
				STMPE1801_MSK_KPC_CMD_SCAN);
	} else {
		ret = stmpe_set_bits(stmpe,
				variant->regs[STMPE_IDX_KPC_CTRL_MSB],
				STMPE_KPC_CTRL_MSB_SCAN_COUNT,
				plat->scan_count << 4);
		if (ret < 0)
			return ret;

		return stmpe_set_bits(stmpe,
				variant->regs[STMPE_IDX_KPC_CTRL_LSB],
				STMPE_KPC_CTRL_LSB_SCAN |
				STMPE_KPC_CTRL_LSB_DEBOUNCE,
				STMPE_KPC_CTRL_LSB_SCAN |
				(plat->debounce_ms << 1));
	}

}

static int __devinit stmpe_keypad_probe(struct platform_device *pdev)
{
	struct stmpe *stmpe = dev_get_drvdata(pdev->dev.parent);
	struct stmpe_keypad_platform_data *plat;
	struct stmpe_keypad *keypad;
	struct input_dev *input;
	int ret;
	int irq;
	int i;

	plat = stmpe->pdata->keypad;
	if (!plat)
		return -ENODEV;

	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		return irq;

	keypad = kzalloc(sizeof(struct stmpe_keypad), GFP_KERNEL);
	if (!keypad)
		return -ENOMEM;

	input = input_allocate_device();
	if (!input) {
		ret = -ENOMEM;
		goto out_freekeypad;
	}

	input->name = "STMPE-keypad";
	input->id.bustype = BUS_I2C;
	input->dev.parent = &pdev->dev;

	input_set_capability(input, EV_MSC, MSC_SCAN);

	__set_bit(EV_KEY, input->evbit);
	if (!plat->no_autorepeat)
		__set_bit(EV_REP, input->evbit);

	input->keycode = keypad->keymap;
	input->keycodesize = sizeof(keypad->keymap[0]);
	input->keycodemax = ARRAY_SIZE(keypad->keymap);

	keypad->stmpe = stmpe;
	keypad->plat = plat;
	keypad->input = input;
	keypad->variant = &stmpe_keypad_variants[stmpe->partnum];

	/*
	 * compute keypad->scan_code_row_shift by figuring out
	 * how many bits are needed to encode keypad->variant->max_cols
	 */
	keypad->scan_code_row_shift =
			get_count_order(keypad->variant->max_cols);

	matrix_keypad_build_keymap(plat->keymap_data,
			keypad->scan_code_row_shift,
			input->keycode, input->keybit);

	for (i = 0; i < plat->keymap_data->keymap_size; i++) {
		unsigned int key = plat->keymap_data->keymap[i];

		keypad->cols |= 1 << KEY_COL(key);
		keypad->rows |= 1 << KEY_ROW(key);
	}

	ret = stmpe_keypad_chip_init(keypad);
	if (ret < 0)
		goto out_freeinput;

	ret = input_register_device(input);
	if (ret) {
		dev_err(&pdev->dev,
			"unable to register input device: %d\n", ret);
		goto out_freeinput;
	}

	ret = request_threaded_irq(irq, NULL, stmpe_keypad_irq, IRQF_ONESHOT,
				   "stmpe-keypad", keypad);
	if (ret) {
		dev_err(&pdev->dev, "unable to get irq: %d\n", ret);
		goto out_unregisterinput;
	}

	/* sysfs implementation for dynamic enable/disable the input event */
	ret = sysfs_create_group(&pdev->dev.kobj, &stmpe_attr_group);
	if (ret) {
		dev_err(&pdev->dev, "failed to create sysfs entries\n");
		goto out_free_irq;
	}

	keypad->enable = true;
	platform_set_drvdata(pdev, keypad);

	return 0;

out_free_irq:
	free_irq(irq, keypad);
out_unregisterinput:
	input_unregister_device(input);
	input = NULL;
out_freeinput:
	input_free_device(input);
out_freekeypad:
	kfree(keypad);
	return ret;
}

static int __devexit stmpe_keypad_remove(struct platform_device *pdev)
{
	struct stmpe_keypad *keypad = platform_get_drvdata(pdev);
	struct stmpe *stmpe = keypad->stmpe;
	int irq = platform_get_irq(pdev, 0);

	stmpe_disable(stmpe, STMPE_BLOCK_KEYPAD);

	sysfs_remove_group(&pdev->dev.kobj, &stmpe_attr_group);
	free_irq(irq, keypad);
	input_unregister_device(keypad->input);
	platform_set_drvdata(pdev, NULL);
	kfree(keypad);

	return 0;
}

#ifdef CONFIG_PM
static int stmpe_keypad_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct stmpe_keypad *keypad = platform_get_drvdata(pdev);
	struct stmpe *stmpe = keypad->stmpe;

	if (!device_may_wakeup(stmpe->dev))
		stmpe_disable(stmpe, STMPE_BLOCK_KEYPAD);

	return 0;
}

static int stmpe_keypad_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct stmpe_keypad *keypad = platform_get_drvdata(pdev);
	struct stmpe *stmpe = keypad->stmpe;

	if (!device_may_wakeup(stmpe->dev))
		stmpe_enable(stmpe, STMPE_BLOCK_KEYPAD);

	return 0;
}

static const struct dev_pm_ops stmpe_keypad_dev_pm_ops = {
	.suspend = stmpe_keypad_suspend,
	.resume = stmpe_keypad_resume,
};
#endif

static struct platform_driver stmpe_keypad_driver = {
	.driver.name	= "stmpe-keypad",
	.driver.owner	= THIS_MODULE,
#ifdef CONFIG_PM
	.driver.pm	= &stmpe_keypad_dev_pm_ops,
#endif
	.probe		= stmpe_keypad_probe,
	.remove		= __devexit_p(stmpe_keypad_remove),
};
module_platform_driver(stmpe_keypad_driver);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("STMPExxxx keypad driver");
MODULE_AUTHOR("Rabin Vincent <rabin.vincent@stericsson.com>");
