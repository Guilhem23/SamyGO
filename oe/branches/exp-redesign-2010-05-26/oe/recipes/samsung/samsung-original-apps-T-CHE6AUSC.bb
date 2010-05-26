FIRMWARE_NAME = "T-CHE6AUSC"
# 1012.0
SRC_URI = "http://downloadcenter.samsung.com/content/FM/200909/20090922132745828/2009_DTV_128M_firmware.exe"
FLASH_RFS_VERSION = "25_64_256-25"

require samsung-original-apps-common.inc

unpack_exe () {
	rm -rf ${D}/mtd_exe
	unsquashfs -dest ${D}/mtd_exe ${FIRMWARE_NAME}/image/exe.img
	rm ${D}/mtd_exe/rc.local
	mv ${D}/mtd_appdata/Comp_LIB ${D}/mtd_exe/
}

unpack_firmware () {
	${STAGING_BINDIR_NATIVE}/unzip x -qq ${WORKDIR}/2009_DTV_128M_firmware.exe
}

SRC_URI[md5sum] = "532c465c55c3368e7cd9e4ae7a684c6d"
SRC_URI[sha256sum] = "c3a84ef70f90cff0232dce7f99907e1714e6a441316519d51dab9dfeccdfce59"
