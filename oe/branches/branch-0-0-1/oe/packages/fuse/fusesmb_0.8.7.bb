DESCRIPTION = "With SMB for Fuse you can seamlessly browse your network neighbourhood as were it on your own filesystem."
AUTHOR = "Vincent Wagelaar <vincent@ricardis.tudelft.nl>"
HOMEPAGE = "http://www.ricardis.tudelft.nl/~vincent/fusesmb/"
SECTION = "console/network"
PRIORITY = "optional"
LICENSE = "GPLv2"
DEPENDS = "glib-2.0 fuse samba"
RDEPENDS += " libsmbclient "
PR = "r03"

SRC_URI = "http://www.ricardis.tudelft.nl/~vincent/fusesmb/download/${P}.tar.gz"
SRC_URI_append_samygo += " file://04_04_fusesmb.init"

S = "${WORKDIR}/${P}"

export ac_cv_prog_NMBLOOKUP=yes

inherit autotools pckgconfig

do_install_append_samygo(){
        install -d ${D}/etc/init.d
        install -m 755 ${WORKDIR}/04_04_fusesmb.init ${D}/etc/init.d
}

