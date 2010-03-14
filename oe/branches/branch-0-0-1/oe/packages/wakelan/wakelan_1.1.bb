DESCRIPTION = "Wakelan sends a magic packet to wake up remote PC's"
LICENSE = "GPL"
PR = "r03"

SRC_URI = "http://www.ibiblio.org/pub/Linux/system/network/misc/${PN}-${PV}.tar.gz"
SRC_URI_append_samygo = " file://02_10_wakelan.init.dis"

inherit autotools

do_install () {
        install -d ${D}/etc/init.d
        install -d ${D}${bindir}
        install -m 0755 ${WORKDIR}/${PN}-${PV}/wakelan ${D}${bindir}/wakelan
        install -m 0755 ${WORKDIR}/02_10_wakelan.init.dis ${D}/etc/init.d/
}

FILES = "${bindir}/wakelan /etc"

INHIBIT_AUTO_STAGE = "1"
