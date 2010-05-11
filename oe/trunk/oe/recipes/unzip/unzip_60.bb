DESCRIPTION = "A (de)compression library for the ZIP format"
SECTION = "console/utils"
LICENSE = "Info-ZIP"
PR = "r1"
SRC_URI = "ftp://ftp.info-zip.org/pub/infozip/src/unzip${PV}.tgz"
S = "${WORKDIR}/unzip${PV}"

export LD = "${CC} ${LDFLAGS}"

do_compile() {
        oe_runmake -f unix/Makefile generic
}

do_install() {
        oe_runmake -f unix/Makefile install prefix=${D}${prefix}
	install -d ${D}${mandir}
	mv ${D}${prefix}/man/* ${D}${mandir}
}

SRC_URI[md5sum] = "62b490407489521db863b523a7f86375"
SRC_URI[sha256sum] = "036d96991646d0449ed0aa952e4fbe21b476ce994abc276e49d30e686708bd37"
