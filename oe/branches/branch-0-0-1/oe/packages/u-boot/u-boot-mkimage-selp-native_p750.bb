require u-boot_${PV}.bb

inherit native

PR = "r1"

PROVIDES = ""
TARGET_LDFLAGS = ""

do_quilt() {
:
}

do_compile () {
#         chmod +x board/neo1973/gta01/split_by_variant.sh
        oe_runmake leonid_config
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

