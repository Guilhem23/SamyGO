DESCRIPTION = "Samsung original apps and data extracted from firmware"
SECTION = "base"
LICENCE = "closed"
DEPENDS = "samsung-firmware squashfs-tools-native"

do_install () {
	install -d ${D}
	install -d ${D}/mtd_exe
	unsquashfs -dest ${D}/mtd_appdata ${STAGING_DIR}/share/firmware/${FIRMWARE_NAME}/appdata.img
	mcopy -sQnv -i ${STAGING_DIR}/share/firmware/${FIRMWARE_NAME}/exe.img ::* ${D}/mtd_exe
	rm ${D}/mtd_exe/\$RFS_LOG.LO\$
	chmod +x ${D}/mtd_exe/exeDSP
	chmod +x ${D}/mtd_exe/JadeTarget
}

FILES_${PN} += "/mtd_appdata /mtd_exe"

PACKAGE_ARCH = "all"
