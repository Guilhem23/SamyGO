require u-boot_1.1.6.bb

inherit native

PR = "r0"

PROVIDES = ""
TARGET_LDFLAGS = ""

S = "${WORKDIR}/u-boot-1.1.6"

do_quilt() {
	:
}

do_compile () {
	oe_runmake sdtvchl2_config
	oe_runmake clean
	oe_runmake tools
}

do_stage () {
	install -m 0755 tools/mkimage ${STAGING_BINDIR}/uboot-mkimage
	ln -sf ${STAGING_BINDIR}/uboot-mkimage ${STAGING_BINDIR}/mkimage
}

do_deploy () {
	:
}

