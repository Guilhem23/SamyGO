# require ${PN}.inc
require samba_${PV}.bb
require ${PN}.inc

PR = "r06"

RCONFLICTS = "samba"

CONFFILES_${PN} = "${sysconfdir}/samba/smb.conf"

# The file system settings --foodir=dirfoo and overridden unconditionally
# in the samba config by --with-foodir=dirfoo - even if the --with is not
# specified!  Fix that here.  Set the privatedir to /etc/samba/private.
EXTRA_OECONF += "\
	--with-configdir=${sysconfdir}/samba \
	--with-privatedir=${sysconfdir}/samba/private \
	--with-lockdir=${localstatedir}/lock \
	--with-piddir=${localstatedir}/run \
	--with-logfilebase=${localstatedir}/log \
	--with-libdir=${libdir} \
	--with-mandir=${mandir} \
	"

do_install_append() {
	install -d "${D}${localstatedir}/log"
	rm -f ${D}/sbin/mount.smbfs
	rm -f ${D}/sbin/mount.cifs
	rm -f ${D}/sbin/umount.cifs
	rmdir ${D}/sbin
	install -d "${D}${sysconfdir}/init.d"
	install -d "${D}${sysconfdir}/samba/private"

	install -c -m 755 ${WORKDIR}/init-essential ${D}${sysconfdir}/init.d/samba

	install -d "${D}${sysconfdir}/samba"
}

FILES_${PN} = " \
	       ${bindir}/smbclient \
	       /usr/share/samba \
${libdir}libnsl.so.1 \
${libdir}libtdb.so.1 \
${libdir}libtalloc.so.1 \
	       /etc"
