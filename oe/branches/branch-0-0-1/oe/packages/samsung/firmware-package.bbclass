FILES_${PN} = ${MACHINE}

do_patch () {
        echo "############ Install from package.class #################"
	echo "$(pwd) AT -> ${P_OFFSET} Line -> ${P_LINE} SOURCE -> ${S}"
	cd "${S}"
	ls -l ./
	echo "### Big fat warning! sh4 (& unencoded appdata + exe img) arch is to implement!!"
	for i in exe.img appdata.img ; do
		# cip decrypt if .sec file present
		if [ -e ${MACHINE}/image/$i.sec ] ; then
		# mprotect's decrypt_update tool
		#	decrypt_update ${MACHINE}/image/$i.sec ${MACHINE}/image/$i
		# or decrypt with openssl
			ENDE=$(stat ${MACHINE}/image/$i.sec | grep Size: | cut -d ":" -f2 | cut -f1)
			SHORT=$((${ENDE} - 260))
			dd if=${MACHINE}/image/$i.sec of=${MACHINE}/image/$i.sec.cut bs=${SHORT} count=1	
			openssl aes-128-cbc -d -in ${MACHINE}/image/$i.sec.cut -out ${MACHINE}/image/$i.enc -pass pass:7cb49e2a2fb8c4dd1df3b41e20cbb6179c8bd240
		fi
		crypt-xor -f "${MACHINE}/image/$i.enc" -K "${MACHINE}" -force -q -outfile "${MACHINE}/image/$i"
		rm -f "${MACHINE}/image/$i.enc"
	# extract files for later inspection
	unsquashfs ${MACHINE}/image/$i || $(mkdir tt && mount -o loop,noatime ${MACHINE}/image/$i tt ; cp -a tt/* ./ ; umount tt ; rm -rf tt)
	done
	crypt-xor -f "${MACHINE}/run.sh.enc" -K "${MACHINE}" -force -q -outfile run.sh || true
	# method 1.
	# mkdir tt && mount -o loop,noatime exe.img tt 
	# change whata u want
	# umount tt && rm -rf tt
	# method 2.
	echo "${P_LINE}" | dd conv=notrunc of="${MACHINE}/image/exe.img" seek=${P_OFFSET} bs=1
	# end of methods
	cksfv -b ${MACHINE}/image/*.img > ${MACHINE}/image/validinfo.txt
	for i in exe.img appdata.img ; do
                crypt-xor -f "${MACHINE}/image/$i" -K "${MACHINE}" -force -q -outfile "${MACHINE}/image/$i.enc"
		if [ -e ${MACHINE}/image/$i.sec ] ; then
			openssl aes-128-cbc -e -in ${MACHINE}/image/$i.enc -out ${MACHINE}/image/$i.sec.nosig -pass pass:7cb49e2a2fb8c4dd1df3b41e20cbb6179c8bd240 -debug
		fi
#		rm -f "${MACHINE}/image/$i"
		mv "${MACHINE}/image/$i" ./
		mkdir -p ./Patched || true
		unsquashfs -dest ./Patched ./$i || $(mkdir -p ./tt && mount -o loop,noatime ./$i ./tt ; cp -a tt/* ./Patched/ ; umount tt ; rm -rf tt)
        done
	
}

do_install() {
	echo "############ Install from package.class #################"
	echo "SOURCE ${S} DEST ${D} WORK ${WORKDIR}"
	mkdir -p ${D}
#	for i in ${S}/* ; do
		cp -a ${S}/${MACHINE} ${D}/
#	done
}

