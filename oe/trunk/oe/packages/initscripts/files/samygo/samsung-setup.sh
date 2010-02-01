#! /bin/sh

PATH=/sbin:/bin:/usr/sbin:/usr/bin

mkdir /dev/sam
mount -t tmpfs none /dev/sam -o size=1K,mode=1777
mount -t tmpfs none /dtv -o size=10M,mode=1777
mount -t tmpfs none /mtd_ram -o size=10M,mode=1777

# copy original fsr and rfs modules
mount -t squashfs /dev/tbml7 /mtd_boot
cp /media/hdd/modules/fsr.ko /lib/modules/2.6.18_SELP-ARM/kernel/drivers/fsr/
cp /media/hdd/modules/fsr_stl.ko /lib/modules/2.6.18_SELP-ARM/kernel/drivers/fsr/
cp /media/hdd/modules/rfs.ko /lib/modules/2.6.18_SELP-ARM/kernel/fs/rfs1g/
umount /mtd_boot

modprobe fsr
modprobe fsr_stl
modprobe rfs

mount -t rfs /dev/stl0/14 /mtd_rwarea
if [ $? != 0 ] ; then
	echo "Error mounting /mtd_rwarea"
	exit 1
fi

#SWITCH_FLAG0=/mtd_rwarea/PartitionSwitch_0_0
#SWITCH_FLAG1=/mtd_rwarea/PartitionSwitch_1_0

#if [ -e $SWITCH_FLAG0 ]; then
#	echo $SWITCH_FLAG0 " is found..."
#	mount -t rfs -r /dev/tbml8 /mtd_exe/
#	mount -t squashfs /dev/tbml9 /mtd_appdata/
#elif [ -e $SWITCH_FLAG1 ]; then
#	echo $SWITCH_FLAG1 " is found..."
#	mount -t rfs -r /dev/tbml10 /mtd_exe/
#	mount -t squashfs /dev/tbml11 /mtd_appdata/
#fi

echo /dtv/core > /proc/sys/kernel/core_pattern
echo "32" > /proc/sys/kernel/msgmni

insmod /mtd_drv/samdrv.ko

: exit 0
