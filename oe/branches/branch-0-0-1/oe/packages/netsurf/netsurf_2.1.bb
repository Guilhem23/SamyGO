DESCRIPTION = "Lightweight web browser capable of handling many of the \
web standards in use today."
HOMEPAGE = "http://www.netsurf-browser.org/"
SECTION = "x11/network"
LICENSE = "GPLv2"

SRC_URI = "http://www.netsurf-browser.org/downloads/releases/netsurf-${PV}-src.tar.gz \
	   file://fix_makefile.patch;patch=1 \
	   file://debugxml_fix.patch;patch=1 \
	   file://netsurf.png \
	   file://netsurf.desktop \
	   file://Makefile.config"

# Workaround for 2.1 tarball (unpacks into netsurf/, not netsurf-2.1/ )
S = "${WORKDIR}/netsurf"

DEPENDS = "gtk+ lemon-native re2c-native jpeg openssl curl libxml2 \
	   libglade hubbub libnsgif libnsbmp lcms"

DEPENDS_samygo = "virtual/libsdl lemon-native re2c-native jpeg openssl curl libxml2 \
	   libnsbmp hubbub libnsgif lcms"
# LDFLAGS_samygo += " --unresolved-symbols=ignore-all"

EXTRA_OEMAKE = "CURDIR=${S} DESTDIR=${D} PREFIX=${prefix} WARNFLAGS='-Wno-error -fgnu89-inline'"
LDFLAGS_samygo =+ " -L${STAGING_LIBDIR} -shared -fPIC " 
EXTRA_OEMAKE_append_samygo = " SUBTARGET=-sdl.so GCCSDK_INSTALL_ENV="${STAGING_LIBDIR}/../" TARGET=framebuffer NETSURF_FB_FRONTEND=sdl"

do_configure() {
	cp ${WORKDIR}/Makefile.config ${S}/
}

do_install() {
	oe_runmake install
	install -d ${D}/${datadir}/applications
	install -d ${D}/${datadir}/pixmaps
	install -m 0644 ${WORKDIR}/netsurf.png ${D}/${datadir}/pixmaps/
	install -m 0644 ${WORKDIR}/netsurf.desktop ${D}/${datadir}/applications/
}

