DESCRIPTION = "Virtual Machine for several classic graphical point-and-click adventure games"
HOMEPAGE = "http://www.scummvm.org"
SECTION = "games"
PRIORITY = "optional"
LICENSE = "GPL"
PR = "r0"

#SRC_URI = "${SOURCEFORGE_MIRROR}/scummvm/scummvm-${PV}.tar.bz2"
SRC_URI = "svn://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm;module=trunk;proto=http"

inherit autotools 

EXTRA_OECONF = "--host=${HOST_SYS} \
		--backend=sdl \
		--disable-alsa \
		--prefix=${prefix} \
		--with-ogg-prefix=${STAGING_LIBDIR}/.. \
		--with-tremor-prefix=${STAGING_LIBDIR}/.. \
		"

EXTRA_OEMAKE = "MANDIR=${mandir}"

do_configure() {
	./configure ${EXTRA_OECONF}
}
#		--with-sdl-prefix=${STAGING_BINDIR_CROSS} \
