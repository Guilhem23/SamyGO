DESCRIPTION = "Samsung original apps and data extracted from firmware"
SECTION = "base"
LICENCE = "closed"
DEPENDS = "samsung-firmware squashfs-tools-native"

do_install () {
	install -d ${D}
	unsquashfs -dest ${D}/mtd_appdata ${STAGING_DIR}/share/firmware/${FIRMWARE_NAME}/appdata.img
}

FILES_${PN} += "/mtd_appdata"

PACKAGE_ARCH = "all"
