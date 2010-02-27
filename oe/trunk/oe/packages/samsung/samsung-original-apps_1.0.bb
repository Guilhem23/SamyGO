DESCRIPTION = "Samsung original apps and data extracted from firmware"
SECTION = "base"
LICENCE = "closed"
DEPENDS = "samsung-firmware squashfs-tools-native mtools-native"

require firmware-${FIRMWARE_NAME}.inc

SRC_URI = "file://T_Library.swf"

do_configure () {
	install -m 0644 ${WORKDIR}/T_Library.swf ${S}/
}

do_install () {
	install -d ${D}
	install -d ${D}/mtd_exe
	unsquashfs -dest ${D}/mtd_appdata ${STAGING_DIR}/share/firmware/${FIRMWARE_NAME}/appdata.img
	mcopy -sQnv -i ${STAGING_DIR}/share/firmware/${FIRMWARE_NAME}/exe.img ::* ${D}/mtd_exe
	rm ${D}/mtd_exe/\$RFS_LOG.LO\$
	chmod +x ${D}/mtd_exe/exeDSP
	chmod +x ${D}/mtd_exe/JadeTarget
	echo "${FIRMWARE_NAME}" > ${D}/.info
	echo "${FLASH_RFS_VERSION} ${FIRMWARE_NAME}" > ${D}/.version
	install -d ${D}/mtd_tlib/swf
	install -m 0755 ${S}/T_Library.swf ${D}/mtd_tlib/swf
}

FILES_${PN} += "/mtd_appdata /mtd_exe /mtd_tlib"

PACKAGE_ARCH = "all"
