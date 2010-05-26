FIRMWARE_NAME = "T-CHE7AUSC"
# 1013.0
SRC_URI = "http://downloadcenter.samsung.com/content/FM/200909/20090922135709687/2009_DTV_2G_firmware.exe"
FLASH_RFS_VERSION = "1_2G_64_512-4"

require samsung-original-apps-common.inc

unpack_firmware () {
	${STAGING_BINDIR_NATIVE}/unzip x -qq ${WORKDIR}/2009_DTV_2G_firmware.exe
}

SRC_URI[md5sum] = "1b3b9752418df97ffef29341f071e568"
SRC_URI[sha256sum] = "62bccdbc54526de59be8a08c8c32a81183df42c63f213469f877004ab4534ad2"
