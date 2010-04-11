require xorg-lib-common.inc

DESCRIPTION = "X11 toolkit intrinsics library"
DEPENDS += "libsm virtual/libx11 kbproto"
PROVIDES = "xt"
PR = "r1"
PE = "1"

XORG_PN = "libXt"

EXTRA_OECONF += "--disable-install-makestrs --disable-xkb"

do_compile() {
	(
		unset CC LD CXX CCLD
		oe_runmake -C util 'XT_CFLAGS=' 'CC=${BUILD_CC}' 'CPPFLAGS=' 'CFLAGS=-D_GNU_SOURCE' 'LDFLAGS='  makestrs
	) || exit 1
	oe_runmake
}
