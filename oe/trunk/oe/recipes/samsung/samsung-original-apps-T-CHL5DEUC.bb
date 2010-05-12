FIRMWARE_NAME = "T-CHL5DEUC"
# 2008.0
SRC_URI = "http://downloadcenter.samsung.com/content/FM/200910/20091030222436890/T-CHL5DEUC.exe"
FLASH_RFS_VERSION = "28_64_256-28"

require samsung-original-apps-common.inc

unpack_exe () {
	rm -rf ${D}/mtd_exe
	unsquashfs -dest ${D}/mtd_exe ${FIRMWARE_NAME}/image/exe.img
	rm ${D}/mtd_exe/rc.local
	rm ${D}/mtd_exe/rc.local.rfs
	mv ${D}/mtd_appdata/Comp_LIB ${D}/mtd_exe/
}

unpack_firmware () {
	${STAGING_BINDIR_NATIVE}/unrar x -c- ${WORKDIR}/T-CHL5DEUC.exe
}

SRC_URI[md5sum] = "e12e145a18c94d6706ba7bbe7b04ac82"
SRC_URI[sha256sum] = "2f76e69977998ca48d288e0646fc279277d2b89016661fa8e91363e733eeaafc"
