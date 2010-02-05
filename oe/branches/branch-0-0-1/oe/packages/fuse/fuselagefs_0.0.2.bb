HOMEPAGE = "http://witme.sourceforge.net"
DESCRIPTION = "Broken down into the file manager (ego) and the virtual filesystem (libferris/ferrisui). the VFS libferris is the core of the project, ego uses GTK+2 and gevas to present info from libferris and allow management using gfmv, gfcp, gfrm etc and file clip"
MAINTAINER = "http://sourceforge.net/users/monkeyiq"

LICENSE_${PN} = "GPL"

DEPENDS = "fuse popt libattr" 
RRECOMMENDS_${PN} = "fuse-module"

SRC_URI="${SOURCEFORGE_MIRROR}/project/witme/${PN}/${PV}/${P}.tar.bz2"

inherit autotools autotools_stage pkgconfig

EXTRA_OECONF += " --disable-nls --includedir=${STAGING_INCDIR} "

do_configure() {
        oe_runconf
}

do_stage(){
	autotools_stage_all || true
}
