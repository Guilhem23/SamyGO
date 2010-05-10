FIRMWARE_NAME = "T-CHL7DAUC"
# 2001.1
SRC_URI = "http://downloadcenter.samsung.com/content/FM/200911/20091127101733312/T-CHL7DAUC.exe;md5sum=e2412b2771556c3a77e9601275d283ab"
FLASH_RFS_VERSION = "28_64_512-29"

require samsung-original-apps-common.inc

unpack_firmware () {
	${STAGING_BINDIR_NATIVE}/unzip x -qq ${WORKDIR}/T-CHL7DAUC.exe
}

