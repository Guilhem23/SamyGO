require xorg-lib-common.inc

DESCRIPTION = "X11 toolkit intrinsics library"
DEPENDS += "libsm virtual/libx11 kbproto"
PR = "${INC_PR}.0"
PE = "1"

SRC_URI[archive.md5sum] = "96f3c93434a93186d178b60d4a262496"
SRC_URI[archive.sha256sum] = "706d5248b061c8455f74ce5269ecc5ef59c27d0f52d22b07138f781928dda9ec"

EXTRA_OECONF += "--disable-install-makestrs --disable-xkb"

#SamyGO: fix build makestrs
do_compile() {
	(
		unset CC LD CXX CCLD
		oe_runmake -C util 'XT_CFLAGS=' 'CC=${BUILD_CC}' 'CPPFLAGS=' 'CFLAGS=-D_GNU_SOURCE' 'LDFLAGS='  makestrs
	) || exit 1
	oe_runmake
}

BBCLASSEXTEND = "native"

XORG_PN = "libXt"
