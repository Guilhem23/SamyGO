HOMEPAGE = "http://fuse.sf.net"
DESCRIPTION = "With FUSE it is possible to implement a fully functional filesystem in a userspace program"
MAINTAINER = "Koen Kooi <koen@linuxtogo.org>"

LICENSE_${PN} = "LGPL"

PR = "r01"

DEPENDS = "fakeroot-native"
RRECOMMENDS_${PN} = "fuse-module kernel-module-fuse"

#package utils in a sperate package and stop debian.bbclass renaming it to libfuse-utils, we want it to be fuse-utils 
PACKAGES += "fuse-utils"
FILES_${PN} = "${libdir}/*.so*"
FILES_${PN}-dev += "${libdir}/*.la"
FILES_fuse-utils = "${bindir} ${base_sbindir}"
DEBIAN_NOAUTONAME_fuse-utils = "1"

SRC_URI="${SOURCEFORGE_MIRROR}/fuse/${P}.tar.gz \
		file://dev_misc_fuse.patch;patch=1"

SRC_URI_append_samygo += "file://arm_cache_page_bug_fuse.patch;patch=1 \
			file://dev_misc_fuse_selp.patch;patch=1"

inherit autotools pkgconfig
EXTRA_OECONF = " --disable-kernel-module"

fakeroot do_install() {
oe_runmake install DESTDIR=${D}
}

fakeroot do_stage() {
autotools_stage_all
}


