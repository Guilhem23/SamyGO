DESCRIPTION = "This software provides support for the Tag Image File Format (TIFF)"
LICENSE = "${PN}"
HOMEPAGE = "http://www.remotesensing.org/libtiff/"
DEPENDS = "zlib jpeg lzo"
PV = "3.8.2+4.0.0beta2"

# SRC_URI = "ftp://ftp.remotesensing.org/pub/libtiff/tiff-4.0.0beta2.tar.gz \
SRC_URI = "http://www.vislab.uq.edu.au/ag3/freebsd/distfiles/tiff-4.0.0beta2.tar.gz"

S = "${WORKDIR}/${PN}-4.0.0beta2"

inherit autotools pkgconfig 

PACKAGES =+ "tiffxx tiffxx-dbg tiffxx-dev tiff-utils tiff-utils-dbg"
FILES_tiffxx = "${libdir}/libtiffxx.so.*"
FILES_tiffxx-dev = "${libdir}/libtiffxx.so ${libdir}/libtiffxx.*a"
FILES_tiffxx-dbg += "${libdir}/.debug/libtiffxx.so*"
FILES_tiff-utils = "${bindir}/*"
FILES_tiff-utils-dbg += "${bindir}/.debug/"

do_configure () {
	oe_runconf 
}

do_stage() {
	autotools_stage_all
}
