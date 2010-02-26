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


