DESCRIPTION = "Zlib Compression Library"
SECTION = "libs"
PRIORITY = "required"
HOMEPAGE = "http://www.gzip.org/zlib/"
LICENSE = "zlib"
PR ="r5"

FILESPATH = "${FILE_DIRNAME}/zlib-${PV}"

SRC_URI = "${SOURCEFORGE_MIRROR}/libpng/zlib-${PV}.tar.bz2 \
		file://visibility.patch;patch=1 \
		file://autotools.patch;patch=1 "

S = "${WORKDIR}/zlib-${PV}"

DEPENDS = "libtool-cross"

export CFLAGS = "-fvisibility=hidden"

inherit autotools

EXTRA_OECONF += " --disable-shared"

do_stage() {
	install -m 0644 ${S}/zlib.h ${STAGING_INCDIR}/
	install -m 0644 ${S}/zconf.h ${STAGING_INCDIR}/

	oe_libinstall -a libz ${STAGING_LIBDIR}
}
