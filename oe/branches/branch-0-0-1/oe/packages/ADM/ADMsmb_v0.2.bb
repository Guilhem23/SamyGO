DESCRIPTION = "ADM smb is a security scanner for Samba/LAN Manager Server Message Blocks/Window Shares from the ADM CreW"
HOMEPAGE = "http://www.securityfocus.com/tools/209"
LICENSE = "GPL"

PR = "r02"

inherit pkgconfig autotools

S = "${WORKDIR}/ADM-SAMBA-CLIENT"

SRC_URI = "http://adm.freelsd.net/ADM/${PN}-${PV}.tgz \
		file://ADMsmb_cast.patch;patch=1 \
"

do_install(){
	install -d ${D}${bindir}
	install -m 0755 ADM-smb ${D}${bindir}/
}
