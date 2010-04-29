require u-boot.inc

DEFAULT_PREFERENCE = "-1"

PR = "r10"

SRC_URI = "\
	http://www.samsung.com/global/opensource/files/uboot_${PV}.tgz \
	file://replace_echo_n_b650.patch;patch=1 \
"

COMPATIBLE_MACHINE = "sdp83"

PACKAGE_ARCH = "${MACHINE_ARCH}"
S = ${WORKDIR}/uboot

do_compile () {
        unset LDFLAGS
        unset CFLAGS
        unset CPPFLAGS
        oe_runmake ${UBOOT_MACHINE}
        oe_runmake all
}
