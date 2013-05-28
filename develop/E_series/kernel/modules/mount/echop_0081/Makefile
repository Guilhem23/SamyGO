#
# Makefile for the Linux /dev/pts virtual filesystem.
#

#obj-$(CONFIG_UNIX98_PTYS)		+= devpts.o

#devpts-$(CONFIG_UNIX98_PTYS)		:= inode.o

obj-m += mount.o

KDIR    := /project/user_firmware_T-ECP/VDLinux_2.6.35.11/linux-2.6.35.11/
PWD    := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules;
