FIRMWARE_NAME = "T-CHEAUSC"
# 1012.3
SRC_URI = "http://downloadcenter.samsung.com/content/FM/200909/20090922132250765/2009_DTV_1G_firmware.exe;md5sum=4d6618255c5528b18dd4ef3d49e9aa51"
FLASH_RFS_VERSION = "27_64_512-28"

require samsung-original-apps-common.inc

unpack_firmware () {
	${STAGING_BINDIR_NATIVE}/unzip x -qq ${WORKDIR}/2009_DTV_1G_firmware.exe
}

