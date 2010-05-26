#! /bin/sh

PATH=/sbin:/bin:/usr/sbin:/usr/bin

firmware=`cat "/.info"`

case $firmware in
T-CHE6AUSC|T-CHE7AUSC|T-CHEAUSC|T-CHL5DAUC|T-CHL5DEUC|T-CHL7DAUC|T-CHL7DEUC|T-CHU7DAUC|T-CHU7DEUC)
	mkdir -p /dev/sam
	mount -t tmpfs none /dev/sam -o size=1K,mode=1777
	mount -t tmpfs none /dtv -o size=10M,mode=1777
	mount -t tmpfs none /mtd_ram -o size=10M,mode=1777
	mkdir -p /dtv/usb

	# copy original fsr and rfs modules if they are missing
#	kernel_release=`uname -r`
#	if [ ! -f /lib/modules/$kernel_release/kernel/drivers/fsr/fsr.ko ]; then
#		mount -t squashfs /dev/flash_tbml7 /mtd_boot
#		mkdir -p /lib/modules/$kernel_release/kernel/drivers/fsr/ /lib/modules/$kernel_release/kernel/fs/rfs1g/
#		cp /mtd_boot/modules/fsr.ko /lib/modules/$kernel_release/kernel/drivers/fsr/
#		cp /mtd_boot/modules/fsr_stl.ko /lib/modules/$kernel_release/kernel/drivers/fsr/
#		cp /mtd_boot/modules/rfs.ko /lib/modules/$kernel_release/kernel/fs/rfs1g/
#		umount /mtd_boot
#		depmod -a
#	fi

#	modprobe fsr
#	modprobe fsr_stl
#	modprobe rfs

	#SWITCH_FLAG0=/mtd_rwarea/PartitionSwitch_0_0
	#SWITCH_FLAG1=/mtd_rwarea/PartitionSwitch_1_0

	#if [ -e $SWITCH_FLAG0 ]; then
	#	echo $SWITCH_FLAG0 " is found..."
	#	mount -t rfs -r /dev/flash8 /mtd_exe/
	#	mount -t squashfs /dev/flash9 /mtd_appdata/
	#elif [ -e $SWITCH_FLAG1 ]; then
	#	echo $SWITCH_FLAG1 " is found..."
	#	mount -t rfs -r /dev/flash10 /mtd_exe/
	#	mount -t squashfs /dev/flash11 /mtd_appdata/
	#fi

	echo /dtv/core > /proc/sys/kernel/core_pattern
	echo "32" > /proc/sys/kernel/msgmni

	echo "**********************************************************************"
	echo "Samsung Setup finished."
	echo "You may run samsung-start.sh to launch exeDSP"
	echo "You can also set own exeDSP command for samsung-start.sh script."
	echo "Write it in \"/.custom_exedsp_cmd\" file, example: \"gdb ./exeDSP\" and"
	echo "run samsung-start.sh ."
	echo "**********************************************************************"
	;;

*)
	echo "samsung-setup.sh: Failed, unknown device!"
	exit 1
	;;
esac

: exit 0
