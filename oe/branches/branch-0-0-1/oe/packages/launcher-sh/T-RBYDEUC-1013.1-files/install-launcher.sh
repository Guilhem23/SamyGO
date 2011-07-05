#! /bin/sh

if [ -e /mtd_rwarea/PartitionSwitch_1_0 ]; then
	vers=$(cat /mtd_rwarea/Version.1)
else
	vers=$(cat /mtd_rwarea/Version.1)
fi
if [ "$vers" != SWU_T-RBYDEU_001013_I02_EK000DK000_090417 ]; then
	echo "Wrong version $vers"
	exit 1
fi
if [ ! -x ./elfpatcher -o ! -r inj-dbgin.elfpatch ]; then
	echo "Missing elfpatcher or inj-dbgin.elfpatch"
	exit 2
fi
if [ ! -w /mtd_exe/exeDSP -o ! -x /mtd_boot/MicomCtrl ]; then
	echo "Missing exeDSP or MicomCtrl"
	exit 3
fi

echo
echo "  This script installs libGame.so launcher into exeDSP."
echo
echo "              Now going to patch exeDSP."
echo "TV show will hang for 30 second and then TV set will reboot."
echo
echo "            !!!! DO NOT POWER OFF TV !!!!"
echo " !!!! NO WARRANTY, YOU ARE DOING IT AT YOUR OWN RISK !!!!"
echo
echo "Press Ctrl-C to abort or Return to continue"
read x

cp mtd_exe/* /mtd_exe/
cp mtd_rwarea/libso.autoload /mtd_rwarea/

killall exeDSP
/mtd_boot/MicomCtrl 23
( while sleep 0.8; do echo x > /proc/watchdog; done )&
sleep 3
./elfpatcher -e /mtd_exe/exeDSP inj-dbgin.elfpatch
sync
mount -o remount,ro /mtd_exe
echo "Installation finished. Going to reboot TV in 5 seconds."
sleep 5
/mtd_boot/MicomCtrl 29
/mtd_boot/MicomCtrl 29

