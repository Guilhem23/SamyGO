#!/bin/sh

export CROSS_COMPILE=arm-v7a8v2r2-linux-gnueabi-

NM="$CROSS_COMPILE"nm

cpu="1000"
smp_configs="SMP"
board_types="PV"
mem_maps="1024 "
devs="n"

if [ -d image ]; then
    rm -rf image/debug/*
    rm -rf image/release/*
else
    mkdir -p image/debug/
    mkdir -p image/release/
fi

for dev in $devs; do
for mem_map in $mem_maps; do
for board_type in $board_types; do
for smp_config in $smp_configs; do

	if [ "$board_type" = "Pre1" ] || [ "$board_type" = "Pre2" ]; then
		img="onboot.bin.${board_type}"
		if [ "$mem_map" = "1024" ]; then
			echo "Skip $board_type mem:1024MB"
			continue
		fi
	else
		img="onboot.bin.${mem_map}MB"
        if [ "$board_type" = "DV" ] && [ "$mem_map" = "1024" ]; then
            echo "Skip $board_type mem:1024MB"
            continue
        fi
        if [ "$board_type" = "PV" ] && [ "$mem_map" = "1536" ]; then
            echo "Skip $board_type mem:1536MB"
            continue
        fi
	fi

	if [ "$smp_config" = "NOSMP" ]; then
		img="${img}.nosmp"
	fi

	echo "Building ${img}"
	make distclean
	make clean
	make cpu=$cpu smp_config=$smp_config board_type=$board_type mem_map=$mem_map dev=$dev use_ptm=n

	echo "=============================================================="
	echo "=================Check MMC boundary Start====================="
	echo "=============================================================="

#echo "$NM"

	$NM onboot | grep sdp_mmc_8Kbytes_In_Card > boundary.tmp
	$NM onboot | grep -w _start > text_base.tmp

	read -a mmc_boundary_string < boundary.tmp
	read -a text_base_string < text_base.tmp

	mmc_boundary=${mmc_boundary_string[0]}
	text_base_addr=${text_base_string[0]}

	rm boundary.tmp text_base.tmp

	declare -i mmc_boundary_num
	declare -i text_base_num
	declare -i mmc_boundary_max_size

	mmc_boundary_max_size=10#8192
	mmc_boundary_num=16#${mmc_boundary_string[0]}
	text_base_num=16#${text_base_string[0]}

	echo "mmc_boundary : 0x$mmc_boundary"
	echo "text_base_num : 0x$text_base_addr"
	
	left=`expr $mmc_boundary_num - $text_base_num`
	echo "onboot's mmc code size is $left bytes"
	
	if [ $left -le $mmc_boundary_max_size ]
	then
		echo "mmc boundary is OK : left size is = `expr $mmc_boundary_max_size - $left` bytes"
	else
		echo "[onboot] ERROR mmc boundary is Fail : over size is = `expr $left - $mmc_boundary_max_size` bytes"
		echo "Delete onboot.bin"
		rm onboot.bin
	fi


	echo "=============================================================="
	echo "===================Check MMC boundary End====================="
	echo "=============================================================="
	CMAC_FILE=cmac_gen.Firenze
	IMG_DIR=BIN
	if [ -e $CMAC_FILE  ]
	then
		./cmac_gen.Firenze onboot.bin $img.cmac.img
	fi
	if [ $dev = y ]; then
		mv onboot.bin image/debug/$img
	else
		mv onboot.bin image/release/$img
	fi


done
done
done
done

