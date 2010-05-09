FIRMWARE_NAME = "T-SPHAUSC"
# 1004.0
SRC_URI = "http://homepage.mac.com/leoindc/.Public/Samsung%20firmware/samsung.zip;md5sum=64c0425f0410cb95dd800ec338dbe73c"
FLASH_RFS_VERSION = "28_64_256-28"

require firmiszip.inc
require samsung-original-apps-common.inc

decode_firmware () {
	mv T-SPHAUSC-1004.0 T-SPHAUSC
}

unpack_exe () {
	rm -rf ${D}/mtd_exe
	unsquashfs -dest ${D}/mtd_exe ${FIRMWARE_NAME}/image/exe.img
	rm ${D}/mtd_exe/rc.local
	rm ${D}/mtd_exe/prelink.*
}

unpack_cmm () {
	install -d ${D}/mtd_cmm
	unsquashfs -dest mtd_cmm ${FIRMWARE_NAME}/image/cmm.img
	cp -R mtd_cmm/*_LIB ${D}/mtd_exe/
}

unpack_chip () {
	install -d ${D}/mtd_chip
	unsquashfs -dest mtd_chip ${FIRMWARE_NAME}/image/chip.img
	cp mtd_chip/samdrv.ko ${D}/mtd_exe/
}

