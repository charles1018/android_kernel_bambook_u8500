#
# Makefile for the Linux Bluetooth subsystem.
#

obj-$(CONFIG_BT)	+= bluetooth.o
obj-$(CONFIG_BT_L2CAP)	+= l2cap.o
obj-$(CONFIG_BT_SCO)	+= sco.o
obj-$(CONFIG_BT_RFCOMM)	+= rfcomm/
obj-$(CONFIG_BT_BNEP)	+= bnep/
obj-$(CONFIG_BT_CMTP)	+= cmtp/
obj-$(CONFIG_BT_HIDP)	+= hidp/

bluetooth-y := af_bluetooth.o hci_core.o hci_conn.o hci_event.o mgmt.o hci_sock.o hci_sysfs.o lib.o
l2cap-y := l2cap_core.o l2cap_sock.o
