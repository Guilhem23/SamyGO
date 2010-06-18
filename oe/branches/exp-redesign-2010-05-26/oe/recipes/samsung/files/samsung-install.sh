#!/bin/sh

# - SamyGO -
#
# Copyright (C) 2010 Pawel Kolodziejski (aquadran at users.sourceforge.net)
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.


echo
echo " --- Installer of Samsung Firmware Software ---"
echo " --- SamyGO  http://samygo.sourceforge.net  ---"
echo

while true; do
	echo
	echo "Select proper firmware for your TV:"
	echo
	echo "1. T-CHE6AUSC"
	echo "2. T-CHE7AUSC"
	echo "3. T-CHEAUSC"
	echo "4. T-CHL5DAUC"
	echo "5. T-CHL5DEUC"
	echo "6. T-CHL7DAUC"
	echo "7. T-CHL7DEUC"
	echo "8. T-CHU7DAUC"
	echo "9. T-CHU7DEUC"
	echo

	read -p "Choice: " sel

	if [ $sel -ge "1" ] && [ $sel -le "9" ]; then
		break
	else
		echo "Wrong choice !"
	fi
done

case $sel in
1)	URL="http://downloadcenter.samsung.com/content/FM/200909/20090922132745828/2009_DTV_128M_firmware.exe" # 1012.0
	FILENAME="2009_DTV_128M_firmware.exe"
	MD5SUM="532c465c55c3368e7cd9e4ae7a684c6d"
	TYPE="T-CHE6AUSC"
	UNCOMP="unzip -qq "
	;;
2)	URL="http://downloadcenter.samsung.com/content/FM/200909/20090922135709687/2009_DTV_2G_firmware.exe" # 1013.0
	FILENAME="2009_DTV_2G_firmware.exe"
	MD5SUM="1b3b9752418df97ffef29341f071e568"
	TYPE="T-CHE7AUSC"
	UNCOMP="unzip -qq "
	;;
3)	URL="http://downloadcenter.samsung.com/content/FM/200909/20090922132250765/2009_DTV_1G_firmware.exe" # 1012.3
	FILENAME="2009_DTV_1G_firmware.exe"
	MD5SUM="4d6618255c5528b18dd4ef3d49e9aa51"
	TYPE="T-CHEAUSC"
	UNCOMP="unzip -qq "
	;;
4)	URL="http://downloadcenter.samsung.com/content/FM/200908/20090807131039093/T-CHL5DAUC.exe" # 1008.0
	FILENAME="T-CHL5DAUC.exe"
	MD5SUM="296e9176826a03ad749f7a361764ee01"
	TYPE="T-CHL5DAUC"
	UNCOMP="unzip -qq "
	;;
5)	URL="http://downloadcenter.samsung.com/content/FM/200910/20091030222436890/T-CHL5DEUC.exe" # 2008.0
	FILENAME="T-CHL5DEUC.exe"
	MD5SUM="e12e145a18c94d6706ba7bbe7b04ac82"
	TYPE="T-CHL5DEUC"
	UNCOMP="unrar x -c- "
	;;
6)	URL="http://downloadcenter.samsung.com/content/FM/200911/20091127101733312/T-CHL7DAUC.exe" # 2001.1
	FILENAME="T-CHL7DAUC.exe"
	MD5SUM="e2412b2771556c3a77e9601275d283ab"
	TYPE="T-CHL7DAUC"
	UNCOMP="unzip -qq "
	;;
7)	URL="http://downloadcenter.samsung.com/content/FM/200910/20091030222802906/T-CHL7DEUC.exe" # 2005.0
	FILENAME="T-CHL7DEUC.exe"
	MD5SUM="2cdfe576c619c9ebf6698b22e9965127"
	TYPE="T-CHL7DEUC"
	UNCOMP="unrar x -c- "
	;;
8)	URL="http://downloadcenter.samsung.com/content/FM/200908/20090806125546515/T-CHU7DAUC.exe" # 1008.0
	FILENAME="T-CHU7DAUC.exe"
	MD5SUM="7432b4087ae1ebb4e62ba9156e1d0ccf"
	TYPE="T-CHU7DAUC"
	UNCOMP="unzip -qq "
	;;
9)	URL="http://downloadcenter.samsung.com/content/FM/200911/20091124161213125/T-CHU7DEUC.exe" # 3000.G / 3000.0
	FILENAME="T-CHU7DEUC.exe"
	MD5SUM="6240c8dc881383b12f2b075719474456"
	TYPE="T-CHU7DEUC"
	UNCOMP="unrar x -c- "
	;;
*)
	echo "Unknown firmware type!"
	echo "Exiting..."
	exit 1
	;;
esac

echo
echo "Selected firmware: $TYPE"
echo

MTD_EXE="/mtd_exe"
MTD_APPDATA="/mtd_appdata"
MTD_TLIB="/mtd_tlib"
INFO="/.info"
START="/usr/sbin/samsung-start.sh"

download_firmware() {
	echo
	echo "Downloading $FILENAME ..."
	echo
	wget -c --timeout=30 --tries=5 $URL
	if [ $? != 0 ]; then
		echo "Error downloading: $URL!"
		echo "Exiting..."
		exit 1
	fi
	sum=`md5sum $FILENAME | cut -c1-32`
	if [ "$MD5SUM" != $sum ]; then
		echo "MD5 checksum is wrong for downloaded file: $FILENAME!"
		echo "Exiting..."
		exit 1
	fi
}

if [ ! -e $FILENAME ]; then
	echo "Firmware file '$FILENAME' not found in current directory."
	echo "To prevent download file from Samsung site, copy '$FILENAME'"
	echo "file to current directory ($PWD)."
	echo
	echo "You can quit script here or continue to download file."
	while true; do
		read -p "Choice ('q' - quite, 'd' - download): " sel
		if [ $sel == "q" ]; then
			echo "Exiting..."
			exit 1;
		fi
		if [ $sel == "d" ]; then
			download_firmware
			if [ ! -e $FILENAME ]; then
				echo "Unexpected missing file: $FILENAME after download!"
				echo "Exiting..."
			fi
			break;
		fi
	done
fi

while true; do
	sum=`md5sum $FILENAME | cut -c1-32`
	if [ "$MD5SUM" != $sum ]; then
		echo "MD5 checksum is wrong for $FILENAME in current directory!"
		echo "d) delete file and exit script,"
		echo "c) try continue download,"
		echo "r) delete and download file,"
		read -p "Select: " sel
		if [ $sel == "d" ]; then
			rm -f $FILENAME
			echo "Exiting..."
			exit 1
		fi
		if [ $sel == "r" ]; then
			rm -f $FILENAME
		fi
		download_firmware
		if [ ! -e $FILENAME ]; then
			echo "Unexpected missing file: $FILENAME after download!"
			echo "Exiting..."
			exit 1
		fi
	else
		break
	fi
done

case $TYPE in
T-CHE7AUSC|T-CHEAUSC|T-CHL7DAUC|T-CHL7DEUC|T-CHU7DAUC|T-CHU7DEUC)
	if [ ! -e LaunchCLManager_v0.01.zip ]; then
		echo "Downloading custom T_Library.swf ..."
		wget -c --timeout=30 --tries=5 http://sourceforge.net/projects/samygo/files/SamyGO%20Applications/LaunchCLManager_v0.01.zip/download 2> /dev/null
		if [ $? != 0 ]; then
			echo "Error downloading: LaunchCLManager_v0.01.zip!"
			echo "Copy LaunchCLManager_v0.01.zip to current directory."
			echo "Exiting..."
			exit 1
		fi
	fi
	;;
*)
	;;
esac

rm -rf tmp-work
mkdir -p tmp-work
cd tmp-work

echo "Decompressing Firmware package..."
$UNCOMP ../$FILENAME 1> /dev/null
if [ $? != 0 ]; then
	echo "Error uncompress file: $FILENAME!"
	echo "Exiting..."
	exit 1
fi

echo
echo "Decoding Firmware package..."
echo
for i in exe.img appdata.img ; do
	crypt-xor -f "${TYPE}/image/$i.enc" -K "${TYPE}" -force -q -outfile "${TYPE}/image/$i"
	if [ $? != 0 ]; then
		echo "Error decode file: $TYPE/image/$i.enc!"
		echo "Exiting..."
		exit 1
	fi
	rm -f "${TYPE}/image/$i.enc"
done

rm -rf $MTD_EXE
rm -rf $MTD_APPDATA

echo
echo "Unpacking mtd_exe..."
echo

case $TYPE in
T-CHE7AUSC|T-CHEAUSC|T-CHL7DAUC|T-CHL7DEUC|T-CHU7DAUC|T-CHU7DEUC)
	mkdir -p ${MTD_EXE}
	mcopy -sQnv -i ${TYPE}/image/exe.img ::* ${MTD_EXE} 2> /dev/null
	if [ $? != 0 ]; then
		echo "Error unpack from exe.img!"
		echo "Exiting..."
		exit 1
	fi
	rm ${MTD_EXE}/\$RFS_LOG.LO\$
	rm -f ${MTD_EXE}/rc.local*
	rm -f ${MTD_EXE}/prelink.*
	chmod +x ${MTD_EXE}/exeDSP
	chmod +x ${MTD_EXE}/JadeTarget
	chmod +x ${MTD_EXE}/ddr_margin
	if [ -f ${MTD_EXE}/memalloc ]; then
		chmod +x ${MTD_EXE}/memalloc
	fi
	;;
T-CHE6AUSC|T-CHL5DAUC|T-CHL5DEUC)
	unsquashfs -dest ${MTD_EXE} ${TYPE}/image/exe.img 1> /dev/null
	if [ $? != 0 ]; then
		echo "Error unpack from exe.img!"
		echo "Exiting..."
		exit 1
	fi
	rm -f ${MTD_EXE}/rc.local*
	;;
esac
rm -f ${TYPE}/image/exe.img

echo
echo "Unpacking mtd_appdata..."
echo

unsquashfs -dest ${MTD_APPDATA} ${TYPE}/image/appdata.img 1> /dev/null
if [ $? != 0 ]; then
	echo "Error unpack from appdata.img!"
	echo "Exiting..."
	exit 1
fi
rm -rf ${MTD_APPDATA}/lib

case $TYPE in
T-CHE6AUSC|T-CHL5DAUC|T-CHL5DEUC)
	mv ${MTD_APPDATA}/Comp_LIB ${MTD_EXE}/
	;;
*)
	;;
esac
rm -rf ${TYPE}

echo "${TYPE}" > ${INFO}

case $TYPE in
T-CHE7AUSC|T-CHEAUSC|T-CHL7DAUC|T-CHL7DEUC|T-CHU7DAUC|T-CHU7DEUC)
	echo
	echo "Creating minimal mtd_tlib..."
	echo
	unzip -qq ../LaunchCLManager_v0.01.zip
	mkdir -p ${MTD_TLIB}/swf
	mv T_Library.swf ${MTD_TLIB}/swf/
	;;
*)
	;;
esac

echo "#! /bin/sh

PATH=/sbin:/bin:/usr/sbin:/usr/bin

firmware=\`cat \"/.info\"\`

case \$firmware in
T-CHE6AUSC|T-CHE7AUSC|T-CHEAUSC|T-CHL5DAUC|T-CHL5DEUC|T-CHL7DAUC|T-CHL7DEUC|T-CHU7DAUC|T-CHU7DEUC)
	mkdir -p /mtd_appdata /mtd_boot /mtd_contents /mtd_down /mtd_exe /mtd_ram /mtd_rwarea /mtd_swu /mtd_tlib /mtd_wiselink
	mkdir -p /mtd_exe/Java

	if [ ! -e /Java ]; then
		ln -s /mtd_exe/Java ${D}/Java
	fi

	for i in mtd_chmap mtd_epg mtd_factory mtd_pers mtd_acap ; do
		if [ ! -e /\$i ]; then
			ln -s /mtd_rwarea /\$i
		fi
	done

	for i in mtd_cmmlib mtd_drv ; do
		if [ ! -e /\$i ]; then
			ln -s /mtd_exe /\$i
		fi
	done

	CMMLIB=/mtd_cmmlib/InfoLink/lib
	export MAPLE_DEFAULT_PATH=\$CMMLIB
	export MAPLE_PLUGIN_DATA_PATH=\$CMMLIB

	WIDGETS=/mtd_down/widgets
	export MAPLE_MANAGER_WIDGET_PATH=\$WIDGETS/manager
	export MAPLE_NORMAL_WIDGET_PATH=\$WIDGETS/normal
	export MAPLE_WIDGET_DATA_PATH=/mtd_down
	export MAPLE_WIDGET_INCLUDE_PATH=\$WIDGETS/inc

	export KF_SLEEP_READ=-2
	export KF_NO_INTERACTIVE=1
	export KF_LOG=/dev/null

	echo 30000 > /mtd_rwarea/DelayValue.txt

	cd /mtd_exe/

	echo \"32\" > /proc/sys/kernel/msgmni

	mkdir -p /dev/sam /dtv
	if [ \`mount | grep -c /dev/sam\` == 0 ]; then
		mount -t tmpfs none /dev/sam -o size=1K,mode=1777
	fi
	if [ \`mount | grep -c /dtv\` == 0 ]; then
		mount -t tmpfs none /dtv -o size=10M,mode=1777
	fi
	if [ \`mount | grep -c /mtd_ram\` == 0 ]; then
		mount -t tmpfs none /mtd_ram -o size=10M,mode=1777
	fi

	if [ ! \"\`lsmod | grep samdrv\`\" ]; then
		insmod /mtd_drv/samdrv.ko
	fi
	sync

	export LD_LIBRARY_PATH=\"/lib:/usr/lib:/Java/lib:/mtd_cmmlib/Comp_LIB:/mtd_cmmlib/InfoLink/lib:/mtd_cmmlib/GAME_LIB:/mtd_cmmlib/DRM_LIB:/mtd_cmmlib/YWidget_LIB\"

	if [ -f /.custom_exedsp_cmd ]; then
		CUSTOM_EXEDSP_CMD=\`cat \"/.custom_exedsp_cmd\"\`
	fi
	echo \"*** Starting exeDSP ***\"
	if [ \"\$CUSTOM_EXEDSP_CMD\" ]; then
		\$CUSTOM_EXEDSP_CMD
	else
		./exeDSP
	fi
	echo \"*** Finished exeDSP ***\"
	;;

*)
	echo \"samsung-start.sh: Failed, unknown device!\"
	;;
esac

" > ${START}

chmod +x ${START}

cd ..
rm -rf tmp-work


echo "Finished."
