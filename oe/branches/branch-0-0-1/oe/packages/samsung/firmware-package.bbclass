# variable not exist in old toolchain, so stay compatible (normaly its same as MACHINE)
FIRMWARE_CLASS ?= "${MACHINE}"

FILES_${PN} = ${FIRMWARE_CLASS}
PACKAGE_ARCH = "${MACHINE}"
# ALLOW_EMPTY_${PN} = "1"
ALLOW_EMPTY = "1"

do_patch () {
	# temporary fix
	if [ -d "T-RBYDEU" ] ; then
	 cp -a T-RBYDEU T-RBYDEUC
	fi
	SEC="no" ; ENC="no"
	for i in exe.img appdata.img ; do
		# cip decrypt if .sec file present
		if [ -e ${FIRMWARE_CLASS}/image/$i.sec ] ; then
		# notice that file was sec
		SEC="yes"
		# mprotect's decrypt_update tool
		#	decrypt_update ${FIRMWARE_CLASS}/image/$i.sec ${FIRMWARE_CLASS}/image/$i
		# or decrypt with openssl
			ENDE=$(stat ${FIRMWARE_CLASS}/image/$i.sec | grep Size: | cut -d ":" -f2 | cut -f1)
			SHORT=$((${ENDE} - 260))
			dd if=${FIRMWARE_CLASS}/image/$i.sec of=${FIRMWARE_CLASS}/image/$i.sec.cut bs=${SHORT} count=1	
			openssl aes-128-cbc -d -in ${FIRMWARE_CLASS}/image/$i.sec.cut -out ${FIRMWARE_CLASS}/image/$i.enc \
				-pass pass:7cb49e2a2fb8c4dd1df3b41e20cbb6179c8bd240
			# for T-VAL... (tested just with samsung-firmware_3001.1+Samsung.bb -> ok) 
			# -pass pass:011fa825d88c76c8128b14592d859898008eae79
		fi
		# x-or .enc file if present
		if [ -e ${FIRMWARE_CLASS}/image/$i.enc ] ; then
		# notice that file was x-ored
		ENC="yes"
		crypt-xor -f "${FIRMWARE_CLASS}/image/$i.enc" -K "${FIRMWARE_CLASS}" -force -q -outfile "${FIRMWARE_CLASS}/image/$i"
		rm -f "${FIRMWARE_CLASS}/image/$i.enc"
		fi
	# extract files for later inspection
	mkdir -p ${FIRMWARE_CLASS}-orig
	unsquashfs -i -dest ${FIRMWARE_CLASS}-orig/$i ${FIRMWARE_CLASS}/image/$i || $(mkdir -p ${FIRMWARE_CLASS}-orig/$i ; mcopy -sQnpv -i ${FIRMWARE_CLASS}/image/$i ::* ${FIRMWARE_CLASS}-orig/$i)
	done
	# we can ignore this file, its never used
	# crypt-xor -f "${FIRMWARE_CLASS}/run.sh.enc" -K "${FIRMWARE_CLASS}" -force -q -outfile ${FIRMWARE_CLASS}-orig/run.sh || true
	if [ -n "${P_LINE}" ] ; then
	oenote "P_LINE Present, so patch"
	# method 1.
	# mkdir tt && mount -o loop,noatime exe.img tt 
	# change whata u want
	# method 1.1
	# mcopy -sQnpv -i ${FIRMWARE_CLASS}/image/$i ::* ${FIRMWARE_CLASS}-patched/$i
	# umount tt && rm -rf tt
	# method 2.
	echo "${P_LINE}" | dd conv=notrunc of="${FIRMWARE_CLASS}/image/exe.img" seek=${P_OFFSET} bs=1
	# end of methods
	fi
	# sh4 has this too? we regenerate the file if it was present
	if [ -e ${FIRMWARE_CLASS}/image/validinfo.txt ] ; then
		cksfv -b ${FIRMWARE_CLASS}/image/*.img > ${FIRMWARE_CLASS}/image/validinfo.txt
	fi
	for i in exe.img appdata.img ; do
		unsquashfs -i -dest ${FIRMWARE_CLASS}-patched/$i ${FIRMWARE_CLASS}/image/$i  || $(mkdir -p ${FIRMWARE_CLASS}-patched/$i ; mcopy -sQnpv -i ${FIRMWARE_CLASS}/image/$i ::* ${FIRMWARE_CLASS}-patched/$i)
		if [ $ENC = "yes" ] ; then
		oenote "Firmware was x-ored, regenerate it"
                crypt-xor -f "${FIRMWARE_CLASS}/image/$i" -K "${FIRMWARE_CLASS}" -force -q -outfile "${FIRMWARE_CLASS}/image/$i.enc"
#		rm -f "${FIRMWARE_CLASS}/image/$i"
		mv "${FIRMWARE_CLASS}/image/$i" ./$i-orig
		fi
		if [ -e ${FIRMWARE_CLASS}/image/$i.sec ] ; then
		oenote "Firmware was for ci+, encrypt it again"
		oenote "Don't try to flash it, signature generation method unknown. Image isn't walid!!!!"
			openssl aes-128-cbc -e -in ${FIRMWARE_CLASS}/image/$i.enc -out ${FIRMWARE_CLASS}/image/$i.sec.nosig \
				-pass pass:7cb49e2a2fb8c4dd1df3b41e20cbb6179c8bd240
		fi
#		rm -f "${FIRMWARE_CLASS}/image/$i"
#		mv "${FIRMWARE_CLASS}/image/$i" ./
        done
}

do_patch_T-DT5DEUC(){
	oenote "may somebody like also do some work"
}

do_patch_T-VAL9DEUC(){
	oenote "may somebody like also do some work"
}

do_patch_T-VALDCNC(){
	oenote "may somebody like also do some work"
}

do_configure(){
	# extract some extra image files
	for i in cmm.img chip.img boot.img rootfs.img rocommon.img rwcommon.img ; do
		if [ -e ${FIRMWARE_CLASS}/image/$i ] ; then
			mkdir -p ${FIRMWARE_CLASS}-orig
			unsquashfs -i -dest ${FIRMWARE_CLASS}-orig/$i ${FIRMWARE_CLASS}/image/$i || \
			$(mkdir -p ${FIRMWARE_CLASS}-orig/$i ; mcopy -sQnpv -i ${FIRMWARE_CLASS}/image/$i ::* ${FIRMWARE_CLASS}-orig/$i) 
		fi
	done
}

Y_libs = "libFFMpeg libMMSInput libPluginTvOEM  libTDImage \
libFileInput libmms libTD libFLVDemux libHttpInput libMP3Transform \
libRTMPInput libVideoOutput libGCF libHttpsInput libPCMOutput libRTPInput \
libYahooSDLHWAcceleration libMMFCore libPlaybackEngine libSDAL libYahooTimeManager"

Y_arch = "libRCE libGDM libGeneDebug"

G_libs = "libGPlayerPorting libpngGP libSDL_mixer libSDL_image libSDL libsmpeg libungif"

C_libs = "libavcodec libavformat libavutil libptp libusb"
# temporary disable for ffmpeg and co.
C_libs = "libptp libusb"

I_libs = "libExtention libHttpInput libMMSInput libPlaybackEngine libTDImage \
libFFMpeg libHttpsInput libmms libPl libTD \
libFileInput libHttp libMP3Transform libRTMPInput libVideoOutput \
libAdi libFLVDemux libJi libPCMOutput libRTPInput libWsal \
libAe libGCF libJs libPhnd libSDAL \
libCore libMMFCore libPlApp libSui"

O_apps = "exeDSP JadeTarget.cfg ReleaseInfo SpecialItemNumber.txt ddr_margin \
EXE_IMG_VER LifeScenario rc.local value.bin Factory.dat JadeTarget memalloc rc.local.rfs"

do_install() {
	oenote "############ Install from package.class #################"
	oenote "SOURCE ${S} DEST ${D} WORK ${WORKDIR}"
	mkdir -p ${D}
#	for i in ${S}/* ; do
		cp -a ${S}/${FIRMWARE_CLASS} ${D}/
#	done
}

do_install_append_arm(){
	install -d ${D}${libdir}
	install -d ${D}${base_sbindir}

        if [ "${FIRMWARE_CLASS}" = "T-PRLAUS" -o "${FIRMWARE_CLASS}" = "T-CHL5DEUC" -o "${FIRMWARE_CLASS}" = "T-CHL5DCNC" -o "${FIRMWARE_CLASS}" = "T-VAL9DEUC" ] ; then
        oenote "${FIRMWARE_CLASS} has no libs to stage!!! may other package will fail too"
        else
	
	for i in ${G_libs} ; do
		oe_libinstall -so -C ${FIRMWARE_CLASS}-orig/exe.img/GAME_LIB $i ${D}${libdir} 
	done
	for i in ${C_libs} ; do
		oe_libinstall -so -C ${FIRMWARE_CLASS}-orig/exe.img/Comp_LIB $i ${D}${libdir} 
	done
	for i in ${Y_libs} ; do
		oe_libinstall -so -C ${FIRMWARE_CLASS}-orig/exe.img/YWidget_LIB $i ${D}${libdir}
	done
	for i in ${Y_arch} ; do
		oe_libinstall -a -C ${FIRMWARE_CLASS}-orig/exe.img/YWidget_LIB $i ${D}${libdir}
	done
	for i in ${I_libs} ; do
		oe_libinstall -so -C ${FIRMWARE_CLASS}-orig/exe.img/InfoLink/lib $i ${D}${libdir}
	done
	for i in ${O_apps} ; do
		install -m 755 ${FIRMWARE_CLASS}-orig/exe.img/$i ${D}${base_sbindir} || true
	done
	install -d ${D}/lib/modules/2.x
	install ${FIRMWARE_CLASS}-orig/exe.img/samdrv.ko ${D}/lib/modules/2.x/
	fi

	install -m 755 ${FIRMWARE_CLASS}/MicomCtrl ${D}${base_sbindir} || true
	install -m 755 ${FIRMWARE_CLASS}/crc ${D}${base_sbindir} || true
	install -m 755 ${FIRMWARE_CLASS}/ddcmp ${D}${base_sbindir} || true
	cp -r ${FIRMWARE_CLASS}-orig/exe.img ${D}/mtd_exe || true
	cp -r ${FIRMWARE_CLASS}-orig/appdata.img ${D}/mtd_appdata || true

echo '#!bin/sh
echo "fake $0: $*"
exit 0' > ${D}${base_sbindir}/fsrerase
	chmod 755 ${D}${base_sbindir}/fsrerase
}

do_stage_arm(){

	if [ "${FIRMWARE_CLASS}" = "T-PRLAUS" -o "${FIRMWARE_CLASS}" = "T-CHL5DEUC" -o "${FIRMWARE_CLASS}" = "T-CHL5DCNC" -o "${FIRMWARE_CLASS}" = "T-VAL9DEUC" ] ; then
	oenote "${FIRMWARE_CLASS} has no libs to stage!!! may other package will fail too"
	else	

	for i in ${G_libs} ; do
		oe_libinstall -so -C ${FIRMWARE_CLASS}-orig/exe.img/GAME_LIB $i ${STAGING_LIBDIR}
	done
	for i in ${C_libs} ; do
		oe_libinstall -so -C ${FIRMWARE_CLASS}-orig/exe.img/Comp_LIB $i ${STAGING_LIBDIR} 
	done
	for i in ${Y_libs} ; do
		oe_libinstall -so -C ${FIRMWARE_CLASS}-orig/exe.img/YWidget_LIB $i ${STAGING_LIBDIR}
	done
	for i in ${Y_arch} ; do
		oe_libinstall -a -C ${FIRMWARE_CLASS}-orig/exe.img/YWidget_LIB $i ${STAGING_LIBDIR}
	done
	for i in ${I_libs} ; do
		oe_libinstall -so -C ${FIRMWARE_CLASS}-orig/exe.img/InfoLink/lib $i ${STAGING_LIBDIR}
	done
	fi
}

do_stage(){
	oenote "${TARGET_ARCH} ${MACHINE} has no libs to stage!!! may other package will fail too"
}

PACKAGES =+ "kernel-module-samdrv ${PN}-update-tools ${PN}-theme ${PN}-apps ${PN}-libs ${PN}-appdata ${PN}-exe ${PN}-boot ${PN}-rootfs"
# FILES_${PN} += 
FILES_${PN}-theme = "/mtd_appdata/Images_960x540/WLink_Pro/player/* /mtd_appdata/Images_960x540/InfoLinkCom/Message_error.bmp /mtd_appdata/Images_960x540/ContentsManager/bg/sub_management_bg.bmp /mtd_appdata/Images_960x540/WLink_Pro/new* /mtd_appdata/Images_960x540/DLNACenter/arrow_down.png /mtd_appdata/Images_960x540/DLNACenter/arrow_up.png /mtd_appdata/Images_960x540/DLNACenter/help_enter.png /mtd_appdata/Images_960x540/DLNACenter/help_exit.png /mtd_appdata/Images_960x540/DLNACenter/help_return.png"
FILES_${PN}-theme += " /mtd_appdata/Images_960x540"
FILES_${PN}-appdata = "/mtd_appdata/*"
FILES_${PN}-exe =  "/mtd_exe/*"
FILES_${PN}-boot =  "/mtd_boot/*"
FILES_${PN}-update-tools = "${base_sbindir}/MicomCtrl ${base_sbindir}/crc ${base_sbindir}/ddcmp"
FILES_${PN}-apps = "${base_sbindir}/*"
FILES_${PN}-libs = "${libdir}/*"
FILES_kernel-module-samdrv = "/lib/modules/2.x/*"
FILES_${PN}-rootfs = "/bin/* /sbin/* /usr/* /etc/*"

