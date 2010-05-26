FIRMWARE_NAME = "T-CHEAUSC"
# 1012.3
SRC_URI = "http://downloadcenter.samsung.com/content/FM/200909/20090922132250765/2009_DTV_1G_firmware.exe"
FLASH_RFS_VERSION = "27_64_512-28"

require samsung-original-apps-common.inc

unpack_firmware () {
	${STAGING_BINDIR_NATIVE}/unzip x -qq ${WORKDIR}/2009_DTV_1G_firmware.exe
}

SRC_URI[md5sum] = "4d6618255c5528b18dd4ef3d49e9aa51"
SRC_URI[sha256sum] = "c3bacc49706eeac8b378332b5c409bc759af8e99e31a976f19724a67fea2c860"
