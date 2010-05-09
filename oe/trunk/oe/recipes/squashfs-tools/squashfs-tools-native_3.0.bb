require squashfs-tools_${PV}.bb
PR = "${INC_PR}.1"
DEPENDS = "zlib-native"

inherit native

PACKAGES = ""

do_stage () {
	install -m 0755 mksquashfs ${STAGING_BINDIR}/
	install -m 0755 unsquashfs ${STAGING_BINDIR}/
}
