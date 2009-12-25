#!/bin/sh
# Firmware manipulation utility (unpack/repack)
# needs one parameter subdirectory with firmware name
# like: HandleSamsungTV_Firm.sh T-CHL5DEUC/
# "use the source luke!"

# set -e

PATH=$PATH:/home/Software/Tuxbox/Decompressors
TAR_OPS='--remove-files -rvf'
TAR_OPS='-rvf'

# some_list=$(find $1 -iname *.enc)
# IFS=$'\n';
# for item in $( $some_list; ); do
#	  IFS=;
#	    another_list;
#    done;
#    IFS=;

function ask_type(){
echo $(file -b "$1" | cut -d " " -f 1)
}

echo $BASE
echo find ${1} -name \"*.enc\"
if [ "$?" -eq "0" ] ; then
	echo find ${1} -name \"*.sec\"
fi
echo find ${1} -name \"*.enc\"
if [ "$?" -eq "0" ] ; then
	echo find ${1} -name \"*.sec\"
fi

rm -rf $1/dimage
rm -f $1/*.tar
rm -f $1/*-new.img

	ke=$(echo "$1" | cut -d "/" -f 1)
echo "Set Key to $ke"
# for i in $(find $1 -iname *.enc) ; do
for i in $(find $1 -type f) ; do
	ou=${i/.enc/}
	od="$ke/dimage/$(basename $ou .img)"
	mkdir "$ke/dimage"
	echo "$i" | grep -E ".enc$"
	if [ "$?" -eq "0" ] ; then
		crypt-xor -f "$i" -K "$ke" -force -q -outfile "$ou"
		echo "$i" | grep -E ".sh.enc$"
		if [ "$?" -eq "0" ] ; then
		mv "$ou" "$od"
	fi
	fi
	F_TYPE=$(ask_type "$ou")
	echo "File # $ou # Type -> $F_TYPE"
	case "$F_TYPE" in
		Squashfs)
		rm -rf "$od"
		unsquashfs-3.3-4mdv -dest "$od" "$ou" && rm -f "$ou"
		;;
		x86)
		mkdir tt && mount -o loop,noatime,nodiratime "$ou" tt
		cp -a tt "$ke/dimage/$(basename $ou .img)" 
		umount tt && rm -f "$ou"
		sync
		rm -rf tt
		;;
		POSIX)
		echo "Shell-Script"
		;;
		cannot)
		echo "Shell-Script moved to $od"
		;;
		ASCII)
		echo "Ignore Text File"
		;;
		ELF)
		tar ${TAR_OPS} $ke/${ke}-Samsung-Tools.tar "$ou" 
		;;
		*)
		echo "Unknown File Type -> Ignore"
		;;
	esac

done

pushd "$ke/dimage"
rm -rf exe/tt
find -name "*.so*" -exec tar ${TAR_OPS} ../${ke}-Samsung-dll.tar {} \;
find -name "*.ko" -exec tar ${TAR_OPS} ../${ke}-Samsung-Driver.tar {} \;
tar ${TAR_OPS} ../${ke}-Samsung-Theme.tar appdata/Images_960x540 
tar ${TAR_OPS} ../${ke}-Samsung-Widget.tar appdata/widget 
popd

# Test reassamble
mksquashfs-3.0 $ke/dimage/appdata/ $ke/appimage-new-3.0.img -b 65536 -le -all-root
mksquashfs-3.0 $ke/dimage/rootfs/ $ke/rootfs-new-3.0.img -b 65536 -le -all-root
# mksquashfs-dm $ke/dimage/appdata/ $ke/appimage-new-dm.img -b 65536 -le -all-root
# mksquashfs $ke/dimage/appdata/ $ke/appimage-new.img -b 65536 -le -all-root
