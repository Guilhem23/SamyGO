HOMEPAGE = "http://fuse.sf.net"
DESCRIPTION = "With FUSE it is possible to implement a fully functional filesystem in a userspace program"
MAINTAINER = "Koen Kooi <koen@linuxtogo.org>"

LICENSE = "GPL"


DEPENDS = "fakeroot-native"
RRECOMMENDS = "fuse"

PR = "r03"

FILESDIR = "${@os.path.dirname(bb.data.getVar('FILE',d,1))}/fuse-${PV}"

SRC_URI="${SOURCEFORGE_MIRROR}/fuse/fuse-${PV}.tar.gz \
                file://dev_misc_fuse.patch;patch=1"

SRC_URI_append_samygo += "file://arm_cache_page_bug_fuse.patch;patch=1 \
                        file://dev_misc_fuse_selp.patch;patch=1"

S = "${WORKDIR}/fuse-${PV}"

inherit autotools pkgconfig module

EXTRA_OECONF = "  --with-kernel=${STAGING_KERNEL_DIR} "

# Kernel data not imported??? for what the hell we inherit module??? *Arris *
KERNEL_MAJOR_VERSION = "2.6"

do_configure() {
# cd ${S} ; oe_runconf
	oe_runconf
}

do_compile(){
LDFLAGS=""
cd ${S}/kernel
oe_runmake CC=${KERNEL_CC}
}

fakeroot do_install() {
LDFLAGS=""
cd ${S}/kernel
oe_runmake CC=${KERNEL_CC} install DESTDIR=${D}
}

do_stage(){
	:
}

FILES_${PN} = "/dtv ${base_libdir}/modules"
