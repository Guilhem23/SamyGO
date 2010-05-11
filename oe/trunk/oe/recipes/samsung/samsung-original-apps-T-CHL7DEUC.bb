FIRMWARE_NAME = "T-CHL7DEUC"
# 2005.0
SRC_URI = "http://downloadcenter.samsung.com/content/FM/200910/20091030222802906/T-CHL7DEUC.exe;md5sum=2cdfe576c619c9ebf6698b22e9965127"
# 3000.2
#SRC_URI = "http://downloadcenter.samsung.com/content/FM/201002/20100227161856015/T-CHL7DEUC.exe;m54sum=751d2654fa9ee00ff06b78da7d9019a7"
FLASH_RFS_VERSION = "28_64_512-29"

require samsung-original-apps-common.inc

unpack_firmware () {
	${STAGING_BINDIR_NATIVE}/unrar x -c- ${WORKDIR}/T-CHL7DEUC.exe
}

SRC_URI[md5sum] = "2cdfe576c619c9ebf6698b22e9965127"
SRC_URI[sha256sum] = "d5f16feb073b386738b5b1b70ca667aea9b51b83989be923ed4a91639c52359f"
