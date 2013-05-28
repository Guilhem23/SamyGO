#
# Makefile for the Linux /dev/pts virtual filesystem.
#

obj-m += mount.o


KDIR    := /project/user_firmware_T-MST10/VDLinux_2.6.35.11/linux-2.6.35.11/
PWD    := $(shell pwd)

default:
	cp -f vdlp_version_X10P.h $(KDIR)/include/linux/vdlp_version_X10P.h
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules;
clean:
	make -C $(KDIR) SUBDIRS=$(PWD) clean
	rm -f *.order