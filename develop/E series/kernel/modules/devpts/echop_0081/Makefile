#
# Makefile for the Linux 
#

obj-m += pty.o
modpty-objs :=  pty.o
obj-m += devpts.o

KDIR    := /project/user_firmware_T-ECP/VDLinux_2.6.35.11/linux-2.6.35.11/
PWD    := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules;
clean:
	make -C $(KDIR) SUBDIRS=$(PWD) clean
	rm -f *.order
	

