FIRMWARE_NAME = "T-SPHAUSC"

require samsung-original-apps-common.inc

decode_firmware () {
	mv T-SPHAUSC-1004.0 T-SPHAUSC
}

unpack_exe () {
	rm -rf ${D}/mtd_exe
	unsquashfs -dest ${D}/mtd_exe ${FIRMWARE_NAME}/image/exe.img
	rm ${D}/mtd_exe/rc.local
}
