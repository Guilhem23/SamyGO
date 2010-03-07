DESCRIPTION = "SamyGO Extensions ${PN}"
LICENSE = "GPL"
SECTION = "Gallery/Games"
MAINTAINER = "Ser Lev Arris <arris@ZsoltTech.Com>"
HOMEPAGE = "http://samygo.sourceforge.net"

PR = "r02"

RECOMMENDS = ""
RRECOMMENDS = ""
DEPENDS = ""
RDEPENDS = ""

# from r01
# SRCDATE = "20100201"
SRCDATE = "20100308"

SRC_URI = "svn://samygo.svn.sourceforge.net/svnroot/samygo/SamyGO-Extensions;module=${PN};proto=https"

PACKAGES = "${PN}-dbg ${PN} ${PN}-bin ${PN}-dev ${PN}-doc ${PN}-locale"

# Enable this to generate package per MACHINE
# PACKAGE_ARCH = ${MACHINE_ARCH}
PACKAGE_ARCH = noarch 

S = ${WORKDIR}/${PN}/trunk

do_install () {
	mkdir -p ${D}/etc
	install -m 0755 rcSGO ${D}/
	install -m 0755 rc.sysinit ${D}/etc/
        mkdir -p ${D}/usr/share/doc/${PN}
        cp -a doc/* ${D}/usr/share/doc/${PN}/ || true
}

FILES_${PN} = "/rcSGO /etc/rc.sysinit"
FILES_${PN}-doc = "/usr/share/doc/${PN}"
