FIRMWARE_NAME = "T-CHE6AUSC"

require samsung-original-apps-common.inc

unpack_exe () {
	rm -rf ${D}/mtd_exe
	unsquashfs -dest ${D}/mtd_exe ${FIRMWARE_NAME}/image/exe.img
	rm ${D}/mtd_exe/rc.local
	mv ${D}/mtd_appdata/Comp_LIB ${D}/mtd_exe/
}
