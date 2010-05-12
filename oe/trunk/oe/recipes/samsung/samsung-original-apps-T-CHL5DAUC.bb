FIRMWARE_NAME = "T-CHL5DAUC"
# 1008.0
SRC_URI = "http://downloadcenter.samsung.com/content/FM/200908/20090807131039093/T-CHL5DAUC.exe"
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
	${STAGING_BINDIR_NATIVE}/unzip x -qq ${WORKDIR}/T-CHL5DAUC.exe
}

SRC_URI[md5sum] = "296e9176826a03ad749f7a361764ee01"
SRC_URI[sha256sum] = "9747e0a887300e2750d408a9a64f770a3cc8b12830d4510d19e0f5eb14112510"
