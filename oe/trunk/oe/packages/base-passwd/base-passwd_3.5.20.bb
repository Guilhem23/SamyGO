DESCRIPTION = "Base system password/group files."
SECTION = "base"
LICENSE = "GPL"
PR = "r1"

SRC_URI = "\
  ${DEBIAN_MIRROR}/main/b/base-passwd/base-passwd_${PV}.tar.gz \
  file://nobash.patch;patch=1 \
  file://root-home.patch;patch=1 \
  file://mysql.patch;patch=1 \
"
S = "${WORKDIR}/base-passwd"

inherit autotools

do_install () {
	install -d -m 755 ${D}${sbindir}
	install -p -m 755 update-passwd ${D}${sbindir}/
	install -d -m 755 ${D}${datadir}/base-passwd
	install -p -m 644 passwd.master \
		${D}${datadir}/base-passwd/
	install -p -m 644 group.master \
		${D}${datadir}/base-passwd/
}

do_install_micro () {
	install -d -m 755 ${D}${sysconfdir}
	install -p -m 644 passwd.master ${D}${sysconfdir}/passwd
	install -p -m 644 group.master ${D}${sysconfdir}/group
}

do_install_append_openmn() {
	echo "0:Jn6tcg/qjqvUE:0:0:root:/root:/bin/sh" >>${D}${datadir}/base-passwd/passwd.master
}

pkg_postinst () {
	set -e

	if [ ! -e $D${sysconfdir}/passwd ] ; then
		cp $D${datadir}/base-passwd/passwd.master $D${sysconfdir}/passwd
	fi

	if [ ! -e $D${sysconfdir}/group ] ; then
		cp $D${datadir}/base-passwd/group.master $D${sysconfdir}/group
	fi
	exit 0
}
