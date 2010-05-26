DESCRIPTION = "OPKG Package Manager Utilities"
SECTION = "base"
PRIORITY = "optional"
LICENSE = "GPL"
RDEPENDS = "python"
SRCREV = "4578"
PR = "r2"

#SamyGO: added patch file://allow-uppercase.patch
SRC_URI = "svn://svn.openmoko.org/trunk/src/host/;module=opkg-utils;proto=http \
	file://allow-uppercase.patch;patch=1 "

S = "${WORKDIR}/opkg-utils"

inherit autotools

S = "${WORKDIR}/opkg-utils"

do_stage () {
	autotools_stage_all
}
