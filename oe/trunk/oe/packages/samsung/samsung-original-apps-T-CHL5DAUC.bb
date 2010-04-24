FIRMWARE_NAME = "T-CHL5DAUC"

require samsung-original-apps-common.inc

unpack_exe () {
	rm -rf ${D}/mtd_exe
	unsquashfs -dest ${D}/mtd_exe ${FIRMWARE_NAME}/image/exe.img
	rm ${D}/mtd_exe/rc.local
	rm ${D}/mtd_exe/rc.local.rfs
	mv ${D}/mtd_appdata/Comp_LIB ${D}/mtd_exe/
}
