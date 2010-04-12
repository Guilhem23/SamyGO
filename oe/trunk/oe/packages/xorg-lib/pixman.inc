SECTION = "libs"
PRIORITY = "optional"
DESCRIPTION = "Low-level pixel manipulation library."
LICENSE = "X11"

BBCLASSEXTEND="native"

SRC_URI = "http://cairographics.org/releases/pixman-${PV}.tar.gz;name=archive "

INC_PR = "r5"

inherit autotools pkgconfig
AUTOTOOLS_STAGE_PKGCONFIG = "1"

do_stage() {
	autotools_stage_all
}

