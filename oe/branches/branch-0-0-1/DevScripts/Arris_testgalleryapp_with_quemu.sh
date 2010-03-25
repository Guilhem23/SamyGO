#!/bin/sh

HOME=/mtd_rwarea

# demonstrate the usage of qemu
# fix pathes to fit your setup

cd build ; . ./env.source ; bitbake qemu-native ; cd -

# works too
# build/tmp/staging/i686-linux/bin/qemu-arm -L build/tmp/rootfs/mtd_tlib/SamyGO build/tmp/rootfs/mtd_tlib/SamyGO/usr/bin/smbclient

PATH=$PATH:build/tmp/staging/i686-linux/bin/

echo " Arch $BUILD_CC_ARCH "

TEST=arm # or sh4
# TEST=sh4 # or arm 

case $TEST in
	arm)
	ARC="arm"
	CPU="-cpu cortex-a8"
	MTD_EXE="`pwd`/build/tmp/work/arm-linux-gnueabi/samsung-firmware-3000.0+Samsung-r0/samsung-firmware-3000.0+Samsung/T-CHUCIPDEUC-patched"
	F_ROOT="/home/Software/Tuxbox/Samsung-Firmware/Firmware/T-CHU7DEUC-3000.G"
	LD_SO_P="$(pwd)/build/tmp/rootfs/mtd_tlib/SamyGO/lib/"
	LD_SO_P="$(pwd)/build/tmp/staging/arm-linux-gnueabi/lib/"
	LD_SO="ld-linux.so.3"
	;;
	sh4)
	ARC="sh4"
	CPU=""
	MTD_EXE="/home/Software/Tuxbox/Samsung-Firmware/Firmware/T-RBYDEU/dimage/exe"
	MTD_EXE="`pwd`/build/tmp/work/sh4-linux/samsung-firmware-1013.1+Samsung-r0/samsung-firmware-1013.1+Samsung/T-RBYDEU-patched"
	F_ROOT="/home/Software/Tuxbox/Samsung-Firmware/Firmware/T-RBYDEU"
	LD_SO_P="$(pwd)/build/tmp/staging/sh4-linux/lib/"
	LD_SO="ld-2.5.so"
	;;
esac

export QEMU_STRACE="Yes"
# export QEMU_STRACE="No"
unset QEMU_STRACE
export CURLOPT_VERBOSE="true"

# qemu-$ARC $CPU $LD_SO_P/$LD_SO --library-path "$LD_SO_P:build/tmp/rootfs/mtd_tlib/SamyGO/lib:/home/Video-HD/DLCS/dev_bml0_06/lib:build/tmp/rootfs/lib:build/tmp/rootfs/usr/lib:$MTD_EXE/InfoLink/lib:$MTD_EXE/YWidget_LIB:$MTD_EXE/game_lib:$MTD_EXE/GAME_LIB:$MTD_EXE/Comp_LIB" build/tmp/rootfs/mtd_tlib/SamyGO/usr/bin/curl  -v https://www.google.com
qemu-$ARC $CPU $LD_SO_P/$LD_SO --library-path "$LD_SO_P:build/tmp/rootfs/mtd_tlib/SamyGO/lib:/home/Video-HD/DLCS/dev_bml0_06/lib:build/tmp/rootfs/lib:build/tmp/rootfs/usr/lib:$MTD_EXE/InfoLink/lib:$MTD_EXE/YWidget_LIB:$MTD_EXE/game_lib:$MTD_EXE/GAME_LIB:$MTD_EXE/Comp_LIB" build/tmp/staging/arm-linux-gnueabi/bin/SGexeDSP $(pwd)/build/tmp/rootfs/mtd_tlib/NetSurf/ libNetSurf.so 
# qemu-$ARC $CPU $LD_SO_P/$LD_SO --library-path "$LD_SO_P:build/tmp/rootfs/mtd_tlib/SamyGO/lib:/home/Video-HD/DLCS/dev_bml0_06/lib:build/tmp/rootfs/lib:build/tmp/rootfs/usr/lib:$MTD_EXE/InfoLink/lib:$MTD_EXE/YWidget_LIB:$MTD_EXE/game_lib:$MTD_EXE/GAME_LIB:$MTD_EXE/Comp_LIB" build/tmp/staging/arm-linux-gnueabi/bin/SGexeDSP $(pwd)/build/tmp/rootfs/mtd_tlib/DBowling/ libDBowling.so
# qemu-$ARC $CPU $LD_SO_P/$LD_SO --library-path "$LD_SO_P:build/tmp/rootfs/mtd_tlib/SamyGO/lib:/home/Video-HD/DLCS/dev_bml0_06/lib:build/tmp/rootfs/lib:build/tmp/rootfs/usr/lib:$MTD_EXE/InfoLink/lib:$MTD_EXE/YWidget_LIB:$MTD_EXE/game_lib:$MTD_EXE/GAME_LIB:$MTD_EXE/Comp_LIB" build/tmp/staging/arm-linux-gnueabi/bin/SGexeDSP $(pwd)/build/tmp/rootfs/mtd_tlib/WiseStar/ libWiseStar.so
# qemu-$ARC $CPU $LD_SO_P/$LD_SO --library-path "$LD_SO_P:build/tmp/rootfs/mtd_tlib/SamyGO/lib:/home/Video-HD/DLCS/dev_bml0_06/lib:build/tmp/rootfs/lib:build/tmp/rootfs/usr/lib:$MTD_EXE/InfoLink/lib:$MTD_EXE/YWidget_LIB:$MTD_EXE/game_lib:$MTD_EXE/GAME_LIB:$MTD_EXE/Comp_LIB" ${LD_SO_P}/../bin/SGexeDSP $(pwd)/build/tmp/rootfs/mtd_tlib/SamyGO/ libOverlay.so
# qemu-$ARC $CPU $LD_SO_P/$LD_SO --library-path "$LD_SO_P:build/tmp/rootfs/mtd_tlib/SamyGO/lib:/home/Video-HD/DLCS/dev_bml0_06/lib:build/tmp/rootfs/lib:build/tmp/rootfs/usr/lib:$MTD_EXE/InfoLink/lib:$MTD_EXE/YWidget_LIB:$MTD_EXE/game_lib:$MTD_EXE/GAME_LIB:$MTD_EXE/Comp_LIB" build/tmp/staging/arm-linux-gnueabi/bin/SGexeDSP $(pwd)/build/tmp/rootfs/mtd_tlib/SamyGO/ libSamyGO.so

