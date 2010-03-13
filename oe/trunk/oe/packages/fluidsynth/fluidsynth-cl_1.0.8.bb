DESCRIPTION = "Fluidsynth is a software synthesizer"
HOMEPAGE = "http://www.fluidsynth.org/"
SECTION = "libs/multimedia"
LICENSE = "GPLv2"

SRC_URI = "http://savannah.nongnu.org/download/fluid/fluidsynth-${PV}.tar.gz"

inherit autotools pkgconfig lib_package

S = "${WORKDIR}/fluidsynth-${PV}"

#Has broken libtool usage
do_configure() {
	gnu-configize
	oe_runconf
}

do_stage() {
	autotools_stage_all
}

