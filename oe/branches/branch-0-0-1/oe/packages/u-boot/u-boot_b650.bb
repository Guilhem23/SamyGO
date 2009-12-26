require u-boot.inc

DEFAULT_PREFERENCE = "-1"

PR = "r10"

SRC_URI = "\
	http://www.samsung.com/global/opensource/files/uboot_${PV}.tgz \
	file://replace_echo_n_b650.patch;patch=1 \
#	file://selp-new_tools-env.patch;patch=1 \
#	file://selp-onenand_16_read.patch;patch=1 \
"

PACKAGE_ARCH = "${MACHINE_ARCH}"
S = ${WORKDIR}/uboot
# CFLAGS_selp =+ " -DMTD_OLD "

do_compile () {
        unset LDFLAGS
        unset CFLAGS
        unset CPPFLAGS
        oe_runmake ${UBOOT_MACHINE}
        oe_runmake all
# need patch!! Arris        oe_runmake env
}
