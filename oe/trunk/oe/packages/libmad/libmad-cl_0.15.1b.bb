DESCRIPTION = "MPEG Audio Decoder Library"
SECTION = "libs"
PRIORITY = "optional"
LICENSE = "GPL"
PR = "r0"

SRC_URI = "${SOURCEFORGE_MIRROR}/mad/libmad-${PV}.tar.gz \
           file://add-pkgconfig.patch;patch=1 \
	   file://mad.diff;patch=1 \
	   file://mad-mips-h-constraint.patch;patch=1"

S = "${WORKDIR}/libmad-${PV}"

inherit autotools pkgconfig

EXTRA_OECONF = "--enable-speed --disable-shared"
EXTRA_OECONF_append_arm = " --enable-fpm=arm"

do_configure_prepend () {
	touch NEWS AUTHORS ChangeLog
}

do_stage() {
	oe_libinstall -a libmad ${STAGING_LIBDIR}
	install -m 0644 mad.h ${STAGING_INCDIR}
}

ARM_INSTRUCTION_SET = "arm"