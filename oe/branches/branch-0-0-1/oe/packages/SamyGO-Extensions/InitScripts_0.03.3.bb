DESCRIPTION = "SamyGO Extensions ${PN}" 
LICENSE = "GPL"
SECTION = "Gallery/Games"
MAINTAINER = "Ser Lev Arris <arris@ZsoltTech.Com>"
HOMEPAGE = "http://samygo.sourceforge.net"

PR = "r06"

# RECOMMENDS = ""
RRECOMMENDS += " StarterLib CoreScript nfs-utils-discover djmount dropbear vsftpd util-linux-mount util-linux-umount \
		kernel-module-cifs kernel-module-dummy-hcd kernel-module-g-file-storage \
		bluez-utils-nodbus fusesmb wakelan"
# DEPENDS = ""
# RDEPENDS = ""

# r03
SRCDATE = "20100201"
SRCDATE = "20100313"
SRC_URI = "svn://samygo.svn.sourceforge.net/svnroot/samygo/SamyGO-Extensions;module=${PN};proto=https"

PACKAGES = "${PN}-dbg ${PN} ${PN}-bin ${PN}-dev ${PN}-doc ${PN}-locale"

# Enable this to generate package per MACHINE
# PACKAGE_ARCH = ${MACHINE_ARCH}
PACKAGE_ARCH = noarch

S = ${WORKDIR}/${PN}/trunk

do_install () {
	mkdir -p ${D}/etc/init.d
	install -m 0755 init.d/*init* ${D}/etc/init.d/
	mkdir -p ${D}/usr/share/doc/${PN}
	cp -a doc/* ${D}/usr/share/doc/${PN}/
}

FILES_${PN} = "/etc/init.d"
FILES_${PN}-doc = "/usr/share/doc/${PN}"


