STERICCSON_WLAN_BUILT_IN := y
ifeq ($(STERICCSON_WLAN_BUILT_IN),y)

EXTRA_CFLAGS +=  \
        -DCOMPAT_BASE_TREE="\"$(shell cat $(KBUILD_SRC)/net/compat-wireless/compat_base_tree)\"" \
        -DCOMPAT_BASE_TREE_VERSION="\"$(shell cat $(KBUILD_SRC)/net/compat-wireless/compat_base_tree_version)\"" \
        -DCOMPAT_PROJECT="\"Compat-wireless\"" \
        -DCOMPAT_VERSION="\"$(shell cat $(KBUILD_SRC)/net/compat-wireless/compat_version)\""

obj-y += compat.o

else
obj-m += compat.o
endif
#compat-objs :=

obj-$(CONFIG_COMPAT_FIRMWARE_CLASS) += compat_firmware_class.o
obj-$(CONFIG_COMPAT_KFIFO) += kfifo.o

compat-y += main.o

# Compat kernel compatibility code
compat-$(CONFIG_COMPAT_KERNEL_14) += compat-2.6.14.o
compat-$(CONFIG_COMPAT_KERNEL_18) += compat-2.6.18.o
compat-$(CONFIG_COMPAT_KERNEL_19) += compat-2.6.19.o
compat-$(CONFIG_COMPAT_KERNEL_21) += compat-2.6.21.o
compat-$(CONFIG_COMPAT_KERNEL_22) += compat-2.6.22.o
compat-$(CONFIG_COMPAT_KERNEL_23) += compat-2.6.23.o
compat-$(CONFIG_COMPAT_KERNEL_24) += compat-2.6.24.o

compat-$(CONFIG_COMPAT_KERNEL_25) += \
	compat-2.6.25.o \
	pm_qos_params.o

compat-$(CONFIG_COMPAT_KERNEL_26) += compat-2.6.26.o
compat-$(CONFIG_COMPAT_KERNEL_27) += compat-2.6.27.o
compat-$(CONFIG_COMPAT_KERNEL_28) += compat-2.6.28.o
compat-$(CONFIG_COMPAT_KERNEL_29) += compat-2.6.29.o
compat-$(CONFIG_COMPAT_KERNEL_32) += compat-2.6.32.o
compat-$(CONFIG_COMPAT_KERNEL_33) += compat-2.6.33.o
compat-$(CONFIG_COMPAT_KERNEL_35) += compat-2.6.35.o
compat-$(CONFIG_COMPAT_KERNEL_36) += compat-2.6.36.o
compat-$(CONFIG_COMPAT_KERNEL_37) += compat-2.6.37.o
compat-$(CONFIG_COMPAT_KERNEL_38) += compat-2.6.38.o

ifeq ($(STERICCSON_WLAN_BUILT_IN),y)
#obj-y += kfifo.o
compat-y += compat-2.6.36.o
#compat-y += compat-2.6.37.o
#compat-y += compat-2.6.38.o
endif
