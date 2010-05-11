DESCRIPTION = "udev is a daemon which dynamically creates and removes device nodes from \
/dev/, handles hotplug events and loads drivers at boot time. It replaces \
the hotplug package and requires a kernel not older than 2.6.12."
LICENSE = "GPLv2+"

PR = "r21"

# needed for init.d script
RDEPENDS_${PN} += "udev-utils"

SRC_URI = "http://kernel.org/pub/linux/utils/kernel/hotplug/udev-${PV}.tar.gz \
	   file://mount.blacklist \
	   file://run.rules \
	   "
SRC_URI += " \
       file://udev.rules \
       file://devfs-udev.rules \
       file://links.conf \
       file://permissions.rules \
       file://mount.sh \
       file://network.sh \
       file://local.rules \
       file://default \
       file://init \
       file://cache \
"

SRC_URI[md5sum] = "2b2d46db3a0538644312828f09151c13"
SRC_URI[sha256sum] = "e88036fa6560d943acfc75f682940078781429d9d7b506481ec1266d7d864bd7"

inherit update-rc.d autotools_stage

# Put stuff in /lib and /sbin
export sbindir="${base_sbindir}"
export exec_prefix=""
EXTRA_OECONF += " --with-udev-prefix= \
                  --with-libdir-name=${base_libdir} \
"

INITSCRIPT_NAME = "udev"
INITSCRIPT_PARAMS = "start 03 S ."

PACKAGES =+ "udev-utils libvolume-id libvolume-id-dev"

FILES_libvolume-id-dev = "${includedir}/libvolume_id.h ${libdir}/libvolume_id.a ${libdir}/libvolume_id.so ${libdir}/pkgconfig/libvolume_id.pc"
FILES_udev-utils = "${usrbindir}/udevinfo ${sbindir}/udevadm ${usrbindir}/udevtest"
FILES_libvolume-id = "${base_libdir}/libvolume_id.so.*"

RPROVIDES_${PN} = "hotplug"
FILES_${PN} += "${usrbindir}/* ${usrsbindir}/udevd"
FILES_${PN}-dbg += "${usrbindir}/.debug ${usrsbindir}/.debug"

# udev installs binaries under $(udev_prefix)/lib/udev, even if ${libdir}
# is ${prefix}/lib64
FILES_${PN} += "/lib/udev/* /lib/udev/*"
FILES_${PN}-dbg += "/lib/udev/.debug"

do_install () {
	install -d ${D}${usrsbindir} \
		   ${D}${sbindir}
	oe_runmake 'DESTDIR=${D}' INSTALL=install install
	install -d ${D}${sysconfdir}/init.d
	install -m 0755 ${WORKDIR}/init ${D}${sysconfdir}/init.d/udev
	install -m 0755 ${WORKDIR}/cache ${D}${sysconfdir}/init.d/udev-cache

	install -d ${D}${sysconfdir}/default
	install -m 0755 ${WORKDIR}/default ${D}${sysconfdir}/default/udev

	# Move udev rules from $(udev_prefix)/lib to /etc.
	# This is hardcoded to $(udev_prefix)/lib/udev/rules.d in the
	# Makefile, even if libdir is lib64.
	mv ${D}/lib/udev/rules.d ${D}${sysconfdir}/udev/

 	cp ${S}/rules/rules.d/* ${D}${sysconfdir}/udev/rules.d/
	cp ${S}/rules/packages/* ${D}${sysconfdir}/udev/rules.d/

	install -m 0644 ${WORKDIR}/mount.blacklist     ${D}${sysconfdir}/udev/
	install -m 0644 ${WORKDIR}/local.rules         ${D}${sysconfdir}/udev/rules.d/local.rules
	install -m 0644 ${WORKDIR}/permissions.rules   ${D}${sysconfdir}/udev/rules.d/permissions.rules
	install -m 0644 ${WORKDIR}/run.rules          ${D}${sysconfdir}/udev/rules.d/run.rules
	install -m 0644 ${WORKDIR}/udev.rules          ${D}${sysconfdir}/udev/rules.d/udev.rules
	install -m 0644 ${WORKDIR}/links.conf          ${D}${sysconfdir}/udev/links.conf
	if [ "${UDEV_DEVFS_RULES}" = "1" ]; then
		install -m 0644 ${WORKDIR}/devfs-udev.rules ${D}${sysconfdir}/udev/rules.d/devfs-udev.rules
	fi

	touch ${D}${sysconfdir}/udev/saved.uname
	touch ${D}${sysconfdir}/udev/saved.cmdline
	touch ${D}${sysconfdir}/udev/saved.atags

	install -d ${D}${sysconfdir}/udev/scripts/

	install -m 0755 ${WORKDIR}/mount.sh ${D}${sysconfdir}/udev/scripts/mount.sh
	install -m 0755 ${WORKDIR}/network.sh ${D}${sysconfdir}/udev/scripts
}

# Create the cache after checkroot has run
pkg_postinst_${PN}_append() {
update-rc.d $OPT udev-cache start 12 S .

if [ -d $D/lib/udev/rules.d ] ; then
	echo "$D/lib/udev/rules.d is not a symlink, fixing that"
	mv $D/lib/udev/rules.d/* $D${sysconfdir}/udev/rules.d/
	rm -rf $D/lib/udev/rules.d
	ln -sf ${sysconfdir}/udev/rules.d $D/lib/udev/
fi
}

do_stage_append() {
        install -m 0644 ${S}/extras/volume_id/lib/libvolume_id.h ${STAGING_INCDIR}
        oe_libinstall -C extras/volume_id/lib -so libvolume_id ${STAGING_LIBDIR}
        oe_libinstall -C udev/lib -so libudev ${STAGING_LIBDIR}
        # Since we change exec_prefix above, autotools_stage_all will not see the .pc files
        # When we upgrade to 145 with the path bugs fixed we can drop all this (see poky)
        install -d ${STAGING_DIR_TARGET}${prefix}${libdir}/pkgconfig/
        install ${S}/extras/volume_id/lib/libvolume_id.pc ${STAGING_DIR_TARGET}${prefix}${libdir}/pkgconfig/
        install ${S}/udev/lib/libudev.pc ${STAGING_DIR_TARGET}${prefix}${libdir}/pkgconfig/
	install -d ${STAGING_DIR_TARGET}${prefix}${includedir}/
        install ${S}/extras/volume_id/lib/libvolume_id.h ${STAGING_DIR_TARGET}${prefix}${includedir}/
        install ${S}/udev/lib/libudev.h ${STAGING_DIR_TARGET}${prefix}${includedir}/
}
