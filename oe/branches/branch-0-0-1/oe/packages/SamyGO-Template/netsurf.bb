DESCRIPTION = "Netsurf Web-Browser"
LICENSE = "GPL"
SECTION = "Gallery/Others"
HOMEPAGE = "http://sourceforge.net/apps/phpbb/samygo"
MAINTAINER = "https://sourceforge.net/apps/phpbb/samygo/memberlist.php?mode=viewprofile&u=711"

PV = "0.1"
PR = "r01"

DEPENDS = "openssl curl"
# RDEPENDS = "libsdl-gfx0"

# FILES_${PN} = "/lib/*"

# inherit autotools pkgconfig

SRC_URI = "http://rrobek.de/download/netsurf-src.tgz \
#		file://Makefile \
"

S = ${WORKDIR}/${PN}-pc
# EXTRA_OEMAKE = "EXTRA_CFLAGS=-I${STAGING_INCDIR}/SDL"
# EXTRA_CFLAGS += "-I${STAGING_INCDIR}/SDL"

netsurf_dirs = "libparserutils libwapcaplet libnsbmp libnsgif libnsfb libcss hubbub"

do_compile () {
export XML2_CONFIG=${STAGING_BINDIR}/xml2-config
	for i in ${netsurf_dirs} ; do
		cd ${S}/$i && oe_runmake CROSS_COMPILE=${TARGET_PREFIX}
	done
}

do_install () {
      :
}

