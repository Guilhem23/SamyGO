DESCRIPTION = "Hash Calculator"
SECTION = "console/utils"
PV = "1.3.14"
PR = "r0"

FILESPATH = "${@base_set_filespath([ '${FILE_DIRNAME}/cksfv-${PV}', '${FILE_DIRNAME}/files' ], d)}"

SRC_URI = "http://zakalwe.fi/~shd/foss/cksfv/files/cksfv-${PV}.tar.gz"
SRC_URI_append_selp =+ " file://validinfo_format.patch;patch=1"

S = "${WORKDIR}/cksfv-${PV}"

do_configure () {
	./configure	
}

do_compile () {
	oe_runmake 
}

do_install () {
	install -d ${D}/bin
	install -m 0755 src/cksfv ${D}/bin/cksfv	
}
