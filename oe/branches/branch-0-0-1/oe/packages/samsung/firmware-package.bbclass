FILES_${PN} = ${MACHINE}

DEPENDS =+ " mtools-native"

do_patch () {
	# Temporary fix
        oenote "############ Patch from package.class #################"
	oenote "$(pwd) AT -> ${P_OFFSET} Line -> ${P_LINE} SOURCE -> ${S}"
	cd "${S}"
	ls -l ./
	# temporary fix
	if [ -d "T-RBYDEU" ] ; then
	 cp -a T-RBYDEU T-RBYDEUC
	fi
	SEC="no" ; ENC="no"
	for i in exe.img appdata.img ; do
		# cip decrypt if .sec file present
		if [ -e ${MACHINE}/image/$i.sec ] ; then
		# notice that file was sec
		SEC="yes"
		# mprotect's decrypt_update tool
		#	decrypt_update ${MACHINE}/image/$i.sec ${MACHINE}/image/$i
		# or decrypt with openssl
			ENDE=$(stat ${MACHINE}/image/$i.sec | grep Size: | cut -d ":" -f2 | cut -f1)
			SHORT=$((${ENDE} - 260))
			dd if=${MACHINE}/image/$i.sec of=${MACHINE}/image/$i.sec.cut bs=${SHORT} count=1	
			openssl aes-128-cbc -d -in ${MACHINE}/image/$i.sec.cut -out ${MACHINE}/image/$i.enc \
				-pass pass:7cb49e2a2fb8c4dd1df3b41e20cbb6179c8bd240
		fi
		# x-or .enc file if present
		if [ -e ${MACHINE}/image/$i.enc ] ; then
		# notice that file was x-ored
		ENC="yes"
		crypt-xor -f "${MACHINE}/image/$i.enc" -K "${MACHINE}" -force -q -outfile "${MACHINE}/image/$i"
		rm -f "${MACHINE}/image/$i.enc"
		fi
	# extract files for later inspection
	unsquashfs -i -dest ${MACHINE}-orig/$i ${MACHINE}/image/$i || $(mkdir -p ${MACHINE}-orig/$i ; mcopy -sQnv -i ${MACHINE}/image/$i ::* ${MACHINE}-orig/$i)
	done
	# we can ignore this file, its never used
	# crypt-xor -f "${MACHINE}/run.sh.enc" -K "${MACHINE}" -force -q -outfile ${MACHINE}-orig/run.sh || true
	if [ -n "${P_LINE}" ] ; then
	oenote "P_LINE Present, so patch"
	# method 1.
	# mkdir tt && mount -o loop,noatime exe.img tt 
	# change whata u want
	# method 1.1
	# mcopy -sQnv -i ${MACHINE}/image/$i ::* ${MACHINE}-patched/$i
	# umount tt && rm -rf tt
	# method 2.
	echo "${P_LINE}" | dd conv=notrunc of="${MACHINE}/image/exe.img" seek=${P_OFFSET} bs=1
	# end of methods
	fi
	# sh4 has this too? we regenerate the file if it was present
	if [ -e ${MACHINE}/image/validinfo.txt ] ; then
	cksfv -b ${MACHINE}/image/*.img > ${MACHINE}/image/validinfo.txt
	fi
	for i in exe.img appdata.img ; do
		unsquashfs -i -dest ${MACHINE}-patched/$i ${MACHINE}/image/$i  || $(mkdir -p ${MACHINE}-patched/$i ; mcopy -sQnv -i ${MACHINE}/image/$i ::* ${MACHINE}-patched/$i)
		if [ $ENC = "yes" ] ; then
		oenote "Firmware was x-ored, regenerate it"
                crypt-xor -f "${MACHINE}/image/$i" -K "${MACHINE}" -force -q -outfile "${MACHINE}/image/$i.enc"
		fi
		if [ -e ${MACHINE}/image/$i.sec ] ; then
		oenote "Firmware was for ci+, encrypt it again"
		oenote "Don't try to flash it, signature generation method unknown. Image isn't walid!!!!"
			openssl aes-128-cbc -e -in ${MACHINE}/image/$i.enc -out ${MACHINE}/image/$i.sec.nosig \
				-pass pass:7cb49e2a2fb8c4dd1df3b41e20cbb6179c8bd240
		fi
#		rm -f "${MACHINE}/image/$i"
#		mv "${MACHINE}/image/$i" ./
        done
	# extract some extra image files
	for i in cmm.img chip.img boot.img rootfs.img ; do
	if [ -e ${MACHINE}/image/$i ] ; then
		unsquashfs -i -dest ${MACHINE}-orig/$i ${MACHINE}/image/$i || echo "Bummer fo $i"
	fi
	done
	
}

do_install() {
	oenote "############ Install from package.class #################"
	oenote "SOURCE ${S} DEST ${D} WORK ${WORKDIR}"
	mkdir -p ${D}
#	for i in ${S}/* ; do
		cp -a ${S}/${MACHINE} ${D}/
#	done
}

Y_libs = "libFFMpeg libMMSInput libPluginTvOEM  libTDImage \
libFileInput libmms libTD libFLVDemux libHttpInput libMP3Transform \
libRTMPInput libVideoOutput libGCF libHttpsInput libPCMOutput libRTPInput.so \
libYahooSDLHWAcceleration libMMFCore libPlaybackEngine libSDAL libYahooTimeManager"

Y_arch = "libRCE libGDM libGeneDebug"
do_stage_arm(){
	oe_libinstall -so -C ${MACHINE}-orig/exe.img/GAME_LIB libGPlayerPorting ${STAGING_LIBDIR}
	for i in ${Y_libs} ; do
		oe_libinstall -so -C ${MACHINE}-orig/exe.img/YWidget_LIB $i ${STAGING_LIBDIR}
	done
	for i in ${Y_arch} ; do
		oe_libinstall -a -C ${MACHINE}-orig/exe.img/YWidget_LIB $i ${STAGING_LIBDIR}
	done
}
