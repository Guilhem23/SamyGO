FIRMWARE_NAME = "T-CHU7DAUC"
# 1008.0
SRC_URI = "http://downloadcenter.samsung.com/content/FM/200908/20090806125546515/T-CHU7DAUC.exe"
FLASH_RFS_VERSION = "1_2G_64_512-4"

require samsung-original-apps-common.inc

unpack_firmware () {
	${STAGING_BINDIR_NATIVE}/unzip x -qq ${WORKDIR}/T-CHU7DAUC.exe
}

SRC_URI[md5sum] = "7432b4087ae1ebb4e62ba9156e1d0ccf"
SRC_URI[sha256sum] = "26424df725360c2a1981bb3c0565aead3173764c627a497c4ca08a763bb29754"
