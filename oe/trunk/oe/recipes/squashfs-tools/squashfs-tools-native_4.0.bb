require squashfs-tools_4.0.bb

inherit native

DEPENDS = "zlib-native"

PACKAGES = ""

#SamyGO: added unsquashfs
do_stage () {
	install -m 0755 mksquashfs ${STAGING_BINDIR}/
	install -m 0755 unsquashfs ${STAGING_BINDIR}/
}

