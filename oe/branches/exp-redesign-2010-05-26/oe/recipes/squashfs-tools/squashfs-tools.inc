DESCRIPTION = "Squashfs is a highly compressed read-only filesystem for Linux."
SECTION = "base"
LICENSE = "GPLv2"
#SamyGO: remove zlib
DEPENDS = ""
INC_PR = "r3"
#SamyGO: added FILESDIR; removed do_compile

S = "${WORKDIR}/squashfs${@bb.data.getVar('PV',d,1).replace('r','-r')}/squashfs-tools"

FILESDIR = "${@os.path.dirname(bb.data.getVar('FILE',d,1))}/squashfs-tools-${PV}"

# required to share same place with -lzma specific packages
FILESPATHPKG = "squashfs-tools-${PV}:squashfs-tools:files"

SRC_URI ?= "${SOURCEFORGE_MIRROR}/squashfs/squashfs${@bb.data.getVar('PV',d,1).replace('r','-r')}.tar.gz"

prefix = ""

do_install () {
	install -d ${D}${sbindir}
	install -m 0755 mksquashfs ${D}${sbindir}/
	install -m 0755 unsquashfs ${D}${sbindir}/
}
