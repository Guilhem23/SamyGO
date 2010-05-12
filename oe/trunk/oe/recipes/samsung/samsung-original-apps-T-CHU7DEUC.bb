FIRMWARE_NAME = "T-CHU7DEUC"
# 3000.G
SRC_URI = "http://downloadcenter.samsung.com/content/FM/200911/20091124161213125/T-CHU7DEUC.exe"
FLASH_RFS_VERSION = "1_2G_64_512-4"

require samsung-original-apps-common.inc

unpack_firmware () {
	${STAGING_BINDIR_NATIVE}/unrar x -c- ${WORKDIR}/T-CHU7DEUC.exe
}

SRC_URI[md5sum] = "6240c8dc881383b12f2b075719474456"
SRC_URI[sha256sum] = "3fcb682f80ae72aaaa722afe1277b0c9958a62b1a9c3abb7384f2291c3e4a981"
