DESCRIPTION = "Samsung original apps and data extracted from firmware"
SECTION = "base"
LICENCE = "closed"
DEPENDS = "squashfs-tools-native mtools-native crypt-xor-native cksfv-native"
PR = "r0"

require firmware-${FIRMWARE_NAME}.inc

SRC_URI += " file://T_Library.swf"

PACKAGE_ARCH = "all"

INHIBIT_PACKAGE_STRIP = "1"

FILES_${PN} = "/mtd_appdata /mtd_exe /mtd_tlib /.*"

do_configure () {
	install -m 0644 ${WORKDIR}/T_Library.swf ${S}/
}

decode_firmware () {
	for i in exe.img appdata.img ; do
		crypt-xor -f "${FIRMWARE_NAME}/image/$i.enc" -K "${FIRMWARE_NAME}" -force -q -outfile "${FIRMWARE_NAME}/image/$i"
		rm -f "${FIRMWARE_NAME}/image/$i.enc"
	done
	crypt-xor -f "${FIRMWARE_NAME}/run.sh.enc" -K "${FIRMWARE_NAME}" -force -q -outfile ${FIRMWARE_NAME}/run.sh
	rm -f "${FIRMWARE_NAME}/run.sh.enc"
}

unpack_appdata () {
	unsquashfs -dest ${D}/mtd_appdata ${FIRMWARE_NAME}/image/appdata.img
}

unpack_exe () {
	mcopy -sQnv -i ${FIRMWARE_NAME}/image/exe.img ::* ${D}/mtd_exe
	rm ${D}/mtd_exe/\$RFS_LOG.LO\$
	chmod +x ${D}/mtd_exe/exeDSP
	chmod +x ${D}/mtd_exe/JadeTarget
	chmod +x ${D}/mtd_exe/ddr_margin
	chmod +x ${D}/mtd_exe/memalloc
}

do_install () {
	decode_firmware

	install -d ${D}
	install -d ${D}/mtd_exe

	unpack_appdata
	unpack_exe

	install -d ${D}/mtd_tlib/swf
	install -m 0644 ${S}/T_Library.swf ${D}/mtd_tlib/swf

	echo "${FIRMWARE_NAME}" > ${D}/.info
	echo "${FLASH_RFS_VERSION} ${FIRMWARE_NAME}" > ${D}/.version
}

