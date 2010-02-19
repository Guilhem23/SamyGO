DESCRIPTION = "With SMB for Fuse you can seamlessly browse your network neighbourhood as were it on your own filesystem."
AUTHOR = "Vincent Wagelaar <vincent@ricardis.tudelft.nl>"
HOMEPAGE = "http://www.ricardis.tudelft.nl/~vincent/fusesmb/"
SECTION = "console/network"
PRIORITY = "optional"
LICENSE = "GPLv2"
DEPENDS = "glib-2.0 fuse samba"
RDEPENDS += " libsmbclient "
PR = "r01"

SRC_URI = "http://www.ricardis.tudelft.nl/~vincent/fusesmb/download/${P}.tar.gz"

S = "${WORKDIR}/${P}"

export ac_cv_prog_NMBLOOKUP=yes

inherit autotools pckgconfig
