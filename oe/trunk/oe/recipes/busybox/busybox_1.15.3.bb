require busybox.inc
PR = "${INC_PR}.0"

SRC_URI = "\
  http://www.busybox.net/downloads/busybox-${PV}.tar.bz2;name=tarball \
  \
  file://udhcpscript.patch;patch=1 \
  file://udhcpc-fix-nfsroot.patch;patch=1 \
  file://B921600.patch;patch=1 \
  file://get_header_tar.patch;patch=1 \
  file://busybox-appletlib-dependency.patch;patch=1 \
  file://job-control-off.patch;patch=1 \
  file://0000-wget-no-check-certificate.patch;patch=1 \
  file://busybox-cron \
  file://busybox-httpd \
  file://busybox-udhcpd \
  file://busybox-telnetd \
  file://default.script file://simple.script \
  file://hwclock.sh \
  file://mount.busybox \
  file://umount.busybox \
  file://defconfig \
"

SRC_URI_append_samygo = "file://samsung-devices-table.txt"

SRC_URI[tarball.md5sum] = "6059ac9456de6fb18dc8ee4cd0ec9240"
SRC_URI[tarball.sha256sum] = "d74020ad2cc5a4dcc5109c44dbd0e22582d6ce42954b0f1ff29763c8c0ff03cb"
EXTRA_OEMAKE += "V=1 ARCH=${TARGET_ARCH} CROSS_COMPILE=${TARGET_PREFIX}"

do_configure_prepend () {
	if [ "${TARGET_ARCH}" = "avr32" ] ; then
		sed -i s:CONFIG_FEATURE_OSF_LABEL=y:CONFIG_FEATURE_OSF_LABEL=n: ${WORKDIR}/defconfig
	fi
}

do_install_append_samygo() {
	install -m 0644 ${WORKDIR}/samsung-devices-table.txt ${D}${sysconfdir}/
}

