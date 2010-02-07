SECTION = "console/network"
LICENSE = "GPL"
DEPENDS = ""
	       
# SRC_URI += "file://configure-${PV}.patch;patch=1 \
#	file://config-h.patch;patch=1 \
#	file://mtab.patch;patch=1 \
#"

S = "${WORKDIR}/samba-${PV}/source"

inherit autotools

EXTRA_OECONF += '--disable-cups --disable-nls --with-readline=${STAGING_LIBDIR}/.. \
	      --without-ads --without-automount --without-smbmount --without-syslog \
	      --without-ldapsam --without-quotas --without-nisplus-home \
	      --without-sys-quotas --without-winbind \
	      --without-acl-support --without-cups --without-web --without-utils'
	      
do_configure_prepend () {
	./script/mkversion.sh
	#	if [ ! -e acinclude.m4 ]; then
		#	cat aclocal.m4 > acinclude.m4
		#	fi
}

do_compile () {
		#	oe_runmake proto_exists
		base_do_compile
}

do_install(){
	oe_runmake 'DESTDIR=${D}' install	
}

do_install_append() {
	rm -f ${D}${bindir}/*.old
	rm -f ${D}${sbindir}/*.old
	
	mkdir -p ${D}/sbin/
}

do_install_prepend() {

        mkdir -p ${D}/sbin/
       
}

