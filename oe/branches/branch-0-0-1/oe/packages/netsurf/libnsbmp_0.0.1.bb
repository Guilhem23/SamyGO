DESCRIPTION = "Libnsbmp is a decoding library for the BMP and ICO image file formats"
HOMEPAGE = "http://www.netsurf-browser.org/projects/libnsbmp/"
SECTION = "libs"
PRIORITY = "optional"
LICENSE = "MIT"

SRC_URI = "http://www.netsurf-browser.org/projects/releases/libnsbmp-${PV}-src.tar.gz"

inherit pkgconfig

CFLAGS_samygo =+ " -D_POSIX_C_SOURCE=1 -D_XOPEN_SOURCE -D_STRING_ARCH_unaligned -Wno-error"
CPPFLAGS_samygo =+ " -D_POSIX_C_SOURCE=1 -D_XOPEN_SOURCE -D_STRING_ARCH_unaligned -Wno-error"

EXTRA_OEMAKE = "CURDIR=${S} DESTDIR=${D} PREFIX=${prefix} BUILDDIR=build-OE"

# NOTE: we're using default buildmode here, which results in building only
# static libraries (.a) Not a problem as libnsbmp is only used by Netsurf
# at the moment

do_stage() {
	oe_libinstall -a -C build-OE/ libnsbmp ${STAGING_LIBDIR}
	install -m 0644 include/*.h ${STAGING_INCDIR}/
}

do_install() {
	oe_runmake install
}

