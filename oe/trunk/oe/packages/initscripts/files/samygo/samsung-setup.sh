#! /bin/sh

PATH=/sbin:/bin:/usr/sbin:/usr/bin

mkdir /dev/sam
mount -t tmpfs none /dev/sam -o size=1K,mode=1777
mount -t tmpfs none /dtv -o size=10M,mode=1777

SWITCH_FLAG0=/mtd_rwarea/PartitionSwitch_0_0
SWITCH_FLAG1=/mtd_rwarea/PartitionSwitch_1_0

modprobe fsr
modprobe fsr_stl
modprobe rfs

mount -t tmpfs none /mtd_ram -o size=10M,mode=1777

mount -t rfs /dev/stl0/14 /mtd_rwarea
if [ $? != 0 ] ; then
	echo "Error mounting /mtd_rwarea"
	exit 1
fi

if [ -e $SWITCH_FLAG00 ]; then
	echo $SWITCH_FLAG00 " is found..."
	mount -t rfs -r /dev/tbml8 /mtd_exe/
	mount -t squashfs /dev/tbml9 /mtd_appdata/
elif [ -e $SWITCH_FLAG10 ]; then
	echo $SWITCH_FLAG10 " is found..."
	mount -t rfs -r /dev/tbml10 /mtd_exe/
	mount -t squashfs /dev/tbml11 /mtd_appdata/
fi

echo /dtv/core > /proc/sys/kernel/core_pattern
echo "32" > /proc/sys/kernel/msgmni

insmod /mtd_drv/samdrv.ko

export LD_LIBRARY_PATH="/lib:/lib/tls:/usr/lib:/Java/lib:/mtd_cmmlib/Comp_LIB:/mtd_cmmlib/InfoLink/lib:/mtd_cmmlib/GAME_LIB:/mtd_cmmlib/DRM_LIB"


: exit 0
