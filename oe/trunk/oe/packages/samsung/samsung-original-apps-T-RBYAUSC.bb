FIRMWARE_NAME = "T-RBYAUSC"

require samsung-original-apps-common.inc

decode_firmware () {
	:
}

unpack_exe () {
	rm -rf ${D}/mtd_exe
	unsquashfs -dest ${D}/mtd_exe ${FIRMWARE_NAME}/image/exe.img
	rm ${D}/mtd_exe/rc.local
}
