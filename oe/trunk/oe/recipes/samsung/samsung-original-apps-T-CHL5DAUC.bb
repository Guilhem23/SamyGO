FIRMWARE_NAME = "T-CHL5DAUC"
# 1008.0
SRC_URI = "http://downloadcenter.samsung.com/content/FM/200908/20090807131039093/T-CHL5DAUC.exe;md5sum=296e9176826a03ad749f7a361764ee01"
FLASH_RFS_VERSION = "28_64_256-28"

require firmiszip.inc
require samsung-original-apps-common.inc

unpack_exe () {
	rm -rf ${D}/mtd_exe
	unsquashfs -dest ${D}/mtd_exe ${FIRMWARE_NAME}/image/exe.img
	rm ${D}/mtd_exe/rc.local
	rm ${D}/mtd_exe/rc.local.rfs
	mv ${D}/mtd_appdata/Comp_LIB ${D}/mtd_exe/
}

