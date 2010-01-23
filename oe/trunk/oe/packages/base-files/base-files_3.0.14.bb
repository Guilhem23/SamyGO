DESCRIPTION = "Miscellaneous files for the base system."
SECTION = "base"
PRIORITY = "required"
PR = "r60"

LICENSE = "GPL"

SRC_URI = " \
           file://nsswitch.conf \
           file://motd \
           file://inputrc \
           file://host.conf \
           file://profile \
           file://fstab \
	   file://filesystems \
           file://issue.net \
           file://issue \
           file://usbd \
           file://share/dot.bashrc \
           file://share/dot.profile \
           file://licenses/BSD \
           file://licenses/GPL-2 \
           file://licenses/LGPL-2 \
           file://licenses/LGPL-2.1 \
           file://licenses/Artistic "
S = "${WORKDIR}"

docdir_append = "/${P}"
dirs1777 = "/tmp ${localstatedir}/lock ${localstatedir}/tmp"
dirs2775 = "/home ${prefix}/src ${localstatedir}/local"
dirs755 = "/bin /boot /dev ${sysconfdir} ${sysconfdir}/default \
	   ${sysconfdir}/skel /lib /mnt /proc /home/root /sbin \
	   ${prefix} ${bindir} ${docdir} /usr/games ${includedir} \
	   ${libdir} ${sbindir} ${datadir} \
	   ${datadir}/common-licenses ${datadir}/dict ${infodir} \
	   ${mandir} ${datadir}/misc ${localstatedir} \
	   ${localstatedir}/backups ${localstatedir}/cache \
	   ${localstatedir}/lib /sys ${localstatedir}/lib/misc \
	   ${localstatedir}/lock/subsys ${localstatedir}/log \
	   ${localstatedir}/run ${localstatedir}/spool \
	   /mnt /media /media/card /media/cf /media/net /media/ram \
	   /media/union /media/realroot /media/hdd \
           /media/mmc1"
dirs755_append_samygo = " /mtd_appdata /mtd_boot /mtd_contents \
	   /mtd_down /mtd_exe /mtd_ram /mtd_rwarea /mtd_swu \
	   /mtd_tlib /mtd_wiselink /dtv"
conffiles = "${sysconfdir}/debian_version ${sysconfdir}/host.conf \
	     ${sysconfdir}/inputrc ${sysconfdir}/issue /${sysconfdir}/issue.net \
	     ${sysconfdir}/nsswitch.conf ${sysconfdir}/profile \
	     ${sysconfdir}/default"

#
# set standard hostname, might be a candidate for a DISTRO variable? :M:
#
hostname_samygo = "localhost"

do_install () {
	for d in ${dirs755}; do
		install -m 0755 -d ${D}$d
	done
	for d in ${dirs1777}; do
		install -m 1777 -d ${D}$d
	done
	for d in ${dirs2775}; do
		install -m 2755 -d ${D}$d
	done
	for d in card cf net ram; do
		ln -sf /media/$d ${D}/mnt/$d
	done

	if [ -n "${MACHINE}" -a "${hostname}" = "openembedded" ]; then
		echo ${MACHINE} > ${D}${sysconfdir}/hostname
	else
		echo ${hostname} > ${D}${sysconfdir}/hostname
	fi

	install -m 0644 ${WORKDIR}/issue ${D}${sysconfdir}/issue
	install -m 0644 ${WORKDIR}/issue.net ${D}${sysconfdir}/issue.net

	install -m 0644 ${WORKDIR}/fstab ${D}${sysconfdir}/fstab
	install -m 0644 ${WORKDIR}/filesystems ${D}${sysconfdir}/filesystems
	install -m 0644 ${WORKDIR}/usbd ${D}${sysconfdir}/default/usbd
	install -m 0644 ${WORKDIR}/profile ${D}${sysconfdir}/profile
	install -m 0755 ${WORKDIR}/share/dot.profile ${D}${sysconfdir}/skel/.profile
	install -m 0755 ${WORKDIR}/share/dot.bashrc ${D}${sysconfdir}/skel/.bashrc
	install -m 0644 ${WORKDIR}/inputrc ${D}${sysconfdir}/inputrc
	install -m 0644 ${WORKDIR}/nsswitch.conf ${D}${sysconfdir}/nsswitch.conf
	install -m 0644 ${WORKDIR}/host.conf ${D}${sysconfdir}/host.conf
	install -m 0644 ${WORKDIR}/motd ${D}${sysconfdir}/motd

	for license in BSD GPL-2 LGPL-2 LGPL-2.1 Artistic; do
		install -m 0644 ${WORKDIR}/licenses/$license ${D}${datadir}/common-licenses/
	done

	ln -sf /proc/mounts ${D}${sysconfdir}/mtab
}

do_install_append_samygo() {
	mkdir -p ${D}/mtd_exe/Java
	ln -s mtd_exe/Java ${D}/Java

	for i in mtd_chmap mtd_epg mtd_factory mtd_pers mtd_acap ; do
		ln -s mtd_rwarea ${D}/$i
	done	
	for i in mtd_cmmlib mtd_drv ; do
		ln -s mtd_exe ${D}/$i
	done
}

PACKAGES = "${PN}-doc ${PN}"
FILES_${PN} = "/"
FILES_${PN}-doc = "${docdir} ${datadir}/common-licenses"

CONFFILES_${PN} = "${sysconfdir}/fstab ${sysconfdir}/hostname"
