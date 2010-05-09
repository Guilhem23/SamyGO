FIRMWARE_NAME = "T-CHL5DEUC"
# 2008.0
SRC_URI = "http://downloadcenter.samsung.com/content/FM/200910/20091030222436890/T-CHL5DEUC.exe;md5sum=e12e145a18c94d6706ba7bbe7b04ac82"
FLASH_RFS_VERSION = "28_64_256-28"

require firmisrar.inc
require samsung-original-apps-common.inc

unpack_exe () {
	rm -rf ${D}/mtd_exe
	unsquashfs -dest ${D}/mtd_exe ${FIRMWARE_NAME}/image/exe.img
	rm ${D}/mtd_exe/rc.local
	rm ${D}/mtd_exe/rc.local.rfs
	mv ${D}/mtd_appdata/Comp_LIB ${D}/mtd_exe/
}

