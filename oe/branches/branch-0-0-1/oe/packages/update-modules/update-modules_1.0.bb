SECTION = "base"
DESCRIPTION = "Script to manage module configuration files"
LICENSE = "GPLv2"
PACKAGE_ARCH = "all"
PR = "r4"

SRC_URI = "file://update-modules"

pkg_postinst() {
if [ "x$D" != "x" ]; then
	exit 1
fi
update-modules
}

do_install() {
	install -d ${D}${sbindir}
	install ${WORKDIR}/update-modules ${D}${sbindir}
}
