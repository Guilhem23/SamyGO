DESCRIPTION = "Samsung original apps and data extracted from firmware"
SECTION = "base"
LICENCE = "closed"
DEPENDS = "squashfs-tools-native mtools-native crypt-xor-native cksfv-native unrar-native unzip-native"
PR = "r0"

SRC_URI += " file://T_Library.swf"

PACKAGE_ARCH = "${MACHINE_ARCH}"

INHIBIT_PACKAGE_STRIP = "1"

EXCLUDE_FROM_SHLIBS = "1"

PARALLEL_MAKE = ""

FILES_${PN} = "/mtd_appdata /mtd_exe /mtd_tlib"

do_configure () {
	install -m 0644 ${WORKDIR}/T_Library.swf ${S}/
}

decode_firmware () {
	for i in exe.img appdata.img ; do
		crypt-xor -f "${FIRMWARE_NAME}/image/$i.enc" -K "${FIRMWARE_NAME}" -force -q -outfile "${FIRMWARE_NAME}/image/$i"
	done
	crypt-xor -f "${FIRMWARE_NAME}/run.sh.enc" -K "${FIRMWARE_NAME}" -force -q -outfile ${FIRMWARE_NAME}/run.sh
}

unpack_appdata () {
	unsquashfs -dest ${D}/mtd_appdata ${FIRMWARE_NAME}/image/appdata.img
	rm -rf ${D}/mtd_appdata/lib
}

unpack_exe () {
	rm -rf ${D}/mtd_exe
	install -d ${D}/mtd_exe
	mcopy -sQnv -i ${FIRMWARE_NAME}/image/exe.img ::* ${D}/mtd_exe
	rm ${D}/mtd_exe/\$RFS_LOG.LO\$
	rm -f ${D}/mtd_exe/rc.local
	rm -f ${D}/mtd_exe/rc.local.rfs
	rm -f ${D}/mtd_exe/prelink.*
	chmod +x ${D}/mtd_exe/exeDSP
	chmod +x ${D}/mtd_exe/JadeTarget
	chmod +x ${D}/mtd_exe/ddr_margin
	if [ -f ${D}/mtd_exe/memalloc ]; then
		chmod +x ${D}/mtd_exe/memalloc
	fi
}

unpack_cmm () {
	:
}

unpack_chip () {
	:
}

do_install () {
	decode_firmware

	install -d ${D}

	unpack_appdata
	unpack_exe
	unpack_cmm
	unpack_chip

	install -d ${D}/mtd_tlib/swf
	install -m 0644 ${S}/T_Library.swf ${D}/mtd_tlib/swf
}

pkg_postinst_${PN}_append () {
	echo "${FIRMWARE_NAME}" > /.info
	echo "${FLASH_RFS_VERSION} ${FIRMWARE_NAME}" > /.version
}

