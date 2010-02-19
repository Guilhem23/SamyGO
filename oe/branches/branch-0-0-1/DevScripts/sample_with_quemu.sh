#!/bin/sh

# demonstrate the usage of qemu
# fix pathes to fit your setup

TEST=arm # or sh4
# TEST=sh4 # or arm 

case $TEST in
	arm)
	ARC="arm"
	CPU="-cpu cortex-a8"
	MTD_EXE="/home/Software/Tuxbox/Samsung-Firmware/Firmware/T-CHU7DEUC-3000.G/dimage/exe"
	F_ROOT="/home/Software/Tuxbox/Samsung-Firmware/Firmware/T-CHU7DEUC-3000.G"
	LD_SO_P="/home/Video-HD/Chelsea.gnueabi/build/tmp/rootfs/lib"
	LD_SO="ld-linux.so.3"
	;;
	sh4)
	ARC="sh4"
	CPU=""
	MTD_EXE="/home/Software/Tuxbox/Samsung-Firmware/Firmware/T-RBYDEU/dimage/exe"
	F_ROOT="/home/Software/Tuxbox/Samsung-Firmware/Firmware/T-RBYDEU"
	LD_SO_P="/home/Software/Tuxbox/Samsung-Firmware/Firmware/T-RBYDEU/dimage/rootfs/lib"
	LD_SO="ld-2.5.so"
	;;
esac

export QEMU_STRACE="Yes"
# export QEMU_STRACE="No"
# unset QEMU_STRACE

rm -f /dev/ttyS0
# mknod /dev/ttyS0 -m 0666 c 1 136 
# ln -s /dev/ptmx /dev/ttyS0
# are you a screen expert? ;-)
ln -s /dev/pts/1 /dev/ttyS0
# mkfifo /dev/ttyS0
# SH4 FIX
ln -s /dev/ttyS0 /dev/ttyAS2

function MiCo(){
# Je 5x
# T-CHU7DEUC/dimage/run.sh:       $ROOT_DIR/MicomCtrl 23	# Stop Watchdog
# T-CHU7DEUC/dimage/run.sh:       $ROOT_DIR/MicomCtrl 22	#
# T-CHU7DEUC/dimage/run.sh:       $ROOT_DIR/MicomCtrl 143	# Reboot
# T-CHU7DEUC/dimage/run.sh:       $ROOT_DIR/MicomCtrl 22	# ?Sync? 
# T-CHU7DEUC/dimage/run.sh:       $ROOT_DIR/MicomCtrl 18	# ?Shutdown?
# 123 ?Reboot?
echo $0
for i in 23 22 143 22 18 ; do
	for j in a b c d e ; do
		qemu-$ARC $CPU $LD_SO_P/$LD_SO --library-path "$LD_SO_P:/home/Video-HD/Chelsea.gnueabi/build/tmp/rootfs/lib:/home/Video-HD/Chelsea.gnueabi/build/tmp/rootfs/usr/lib:$MTD_EXE/InfoLink/lib:$MTD_EXE/YWidget_LIB:$MTD_EXE/game_lib:$MTD_EXE/Comp_LIB" $F_ROOT/MicomCtrl $i
	done
done
}


# 1. Error:	>>[ERR:TDcChelseaMicomInterface.cpp] t_ReceiveData():452 Micom Receive Data Size Error...Size -1
# 		true, Micom not running
#
# 2. Error:	>>[ERR:TDsEepromStore.cpp] Create():145 NvramCount File Create Error!!
#		ok, but where you wanna create the file?
#		ok, strace say: 1207 stat64("/mtd_rwarea/nvramCount",0x4007edb0) = -1 errno=2 (No such file or directory)
#		ok, create dir /mtd_rwarea to avoid this
mkdir /mtd_rwarea
dd if=/dev/zero of=/mtd_rwarea/nvramCount bs=131072 count=1
# use nvramCount from tv
cp -vf /home/Video-HD/nvramCount /mtd_rwarea/nvramCount
#		but, under trace we see may non fatal but interesting error ->
#			1207 open("/dev/ttyS0",0x102,0) = 3
#			1207 ioctl(3,21515,2,2237,1074261628,1074429336) = -1 errno=5 (Input/output error)
#		ok, we will fix it later
# 3. Error:	1572 open("/dev/sam/gpio0",0x2,010000000000) = -1 errno=2 (No such file or directory)
#		ok, get the device files
#		bummer, file dont exist -> dynamicaly created by samdrv.ko?
# Nodes in Firmware:
# # ll /home/Video-HD/DLCS/squashfs-root/dev/sam/
# # crwxr-xr-x 1 root root 253, 0 2009-01-08 01:45 ae0
# # crwxr-xr-x 1 root root 252, 0 2009-01-08 01:45 aio0
# # crwxr-xr-x 1 root root 254, 0 2009-01-08 01:45 dbg0
# # crwxr-xr-x 1 root root 248, 0 2009-01-08 01:45 h2640
# # crwxr-xr-x 1 root root 249, 0 2009-01-08 01:45 mpeg0
# # crwxr-xr-x 1 root root 249, 1 2009-01-08 01:45 mpeg1
# # crwxr-xr-x 1 root root 251, 0 2009-01-08 01:45 tsd0
# # crwxr-xr-x 1 root root 250, 0 2009-01-08 01:45 tsdqueue0
# Nodes on TV:
# # ls -l /dev/sam/
# # crw-------    1 root     0        233,   0 Jan  1 00:00 Ci0
# # crw-------    1 root     0        239,   0 Jan  1 00:00 IR0
# # crw-------    1 root     0        245,   0 Jan  1 00:00 Jpeg0
# # crw-------    1 root     0        251,   0 Jan  1 00:00 ad0
# # crw-------    1 root     0        250,   0 Jan  1 00:00 ae0
# # crw-------    1 root     0        249,   0 Jan  1 00:00 aio0
# # crw-------    1 root     0        235,   0 Jan  1 00:00 avd0
# # crw-------    1 root     0        253,   0 Jan  1 00:00 dbg0
# # crw-------    1 root     0        243,   0 Jan  1 00:00 dp0
# # crw-------    1 root     0        243,   1 Jan  1 00:00 dp1
# # crw-------    1 root     0        241,   0 Jan  1 00:00 gfx0
# # crw-------    1 root     0        240,   0 Jan  1 00:00 gfx3d0
# # crw-------    1 root     0        242,   0 Jan  1 00:00 gpio0
# # crw-------    1 root     0        244,   0 Jan  1 00:00 incapt0
# # crw-------    1 root     0        244,   1 Jan  1 00:00 incapt1
# # crw-------    1 root     0        252,   0 Jan  1 00:00 mfd0
# # crw-------    1 root     0        246,   0 Jan  1 00:00 mpeg0
# # crw-------    1 root     0        232,   0 Jan  1 00:00 nrss0
# # crw-------    1 root     0        236,   0 Jan  1 00:00 playback0
# # crw-------    1 root     0        237,   0 Jan  1 00:00 record0
# # crw-------    1 root     0        238,   0 Jan  1 00:00 se0
# # crw-------    1 root     0        234,   0 Jan  1 00:00 sys0
# # crw-------    1 root     0        248,   0 Jan  1 00:00 tsd0
# # crw-------    1 root     0        247,   0 Jan  1 00:00 tsdqueue0
# # crw-------    1 root     0        231,   0 Jan  1 00:00 usbfault0
mkdir -p /dev/sam
mknod /dev/sam/gpio0 -m 0666 c 0 242

# 4. Error:	3480 open("/dev/sdp_i2c0",0x1002,010001767110) = -1 errno=2 (No such file or directory)
# Nodes on TV:
# # ls -l /dev/sdp_i2c*
# # crwxr-xr-x    1 root     0        254,   0 Jan  8  2009 /dev/sdp_i2c0
# # crwxr-xr-x    1 root     0        254,   1 Jan  8  2009 /dev/sdp_i2c1
# # crwxr-xr-x    1 root     0        254,   2 Jan  8  2009 /dev/sdp_i2c2
# # crwxr-xr-x    1 root     0        254,   3 Jan  8  2009 /dev/sdp_i2c3
# # crwxr-xr-x    1 root     0        254,   4 Jan  8  2009 /dev/sdp_i2c4
# # crwxr-xr-x    1 root     0        254,   5 Jan  8  2009 /dev/sdp_i2c5
#
#		ok, nodes are present in partitiondump /dev/sdp_i2c[012345]
cp -a /home/Video-HD/DLCS/squashfs-root/dev/sdp_i2c* /dev/

# Non Fatal but interesting:
#		4910 gettimeofday(1074261584,0,13750,110,110000,1074431856) = 0
#		4910 clock_gettime(0,1074261528,1074261496,1074261512,0,1074453456) = 0
#		ok, external lib function (-lrt) may hookable by LD_PRELOAD or 
#			write little proggie to get the fucking time right
# 5. error:	4910 stat64("/mtd_factory/Factory.dat",0x4007ec48) = -1 errno=2 (No such file or directory)
#		4910 open("/mtd_factory/Factory.dat",0,0777) = -1 errno=2 (No such file or directory)      
#		4910 stat64("/mtd_exe/Factory.dat",0x4007ec48) = -1 errno=2 (No such file or directory)    
#		4910 open("/mtd_exe/Factory.dat",0,0777) = -1 errno=2 (No such file or directory)          
#		4910 write(1,0x4890d000,45)File Open Error /mtd_exe/Factory.dat File.. 
#		ok, but write to /mtd_exe ?? mtd_factory -> mtd_rwarea
# # ls -l /mtd_exe/Factory.dat
# # -rwxr-xr-x    1 root     0          297536 Mar 26  2009 /mtd_exe/Factory.dat

ln -s /mtd_rwarea /mtd_factory
dd if=/dev/zero of=/mtd_rwarea/Factory.dat bs=297536 count=1
# result in:
#		6578 stat64("/mtd_factory/Factory.dat",0x4007ec48) = 0
#		6578 open("/mtd_factory/Factory.dat",0,0777) = 5      
#		6578 write(1,0x4890d000,71)############## Find Factory.dat File in mtd_factory ################## = 71
#		6578 read(5,0x400b6478,88) = 88
#		6578 write(1,0x4890d000,1) = 1
#		6578 write(1,0x4890d000,44)@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ = 44
#		6578 write(1,0x4890d000,68)@     Doesn't Match the DBVersion!!!!!!  ... 68
#		6578 write(1,0x4890d000,44)@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
# so try shipped Factory.dat
cp -vf $MTD_EXE/Factory.dat /mtd_rwarea/

# OK, Test ends with:
#		7462 open("/dev/mem",0x1002,0) = 6
#		7462 mmap2(0,11534336,3,1,6,196608) = 0xffffffea
#		qemu: uncaught target signal 11 (Segmentation fault) - exiting
#	Program qemu-arm tried to access /dev/mem between 30000000->30b00000

qemu-$ARC $CPU $LD_SO_P/$LD_SO --library-path "$LD_SO_P:/home/Video-HD/Chelsea.gnueabi/build/tmp/rootfs/lib:/home/Video-HD/Chelsea.gnueabi/build/tmp/rootfs/usr/lib:$MTD_EXE/InfoLink/lib:$MTD_EXE/YWidget_LIB:$MTD_EXE/game_lib:$MTD_EXE/Comp_LIB" $MTD_EXE/exeDSP

# MiCo
