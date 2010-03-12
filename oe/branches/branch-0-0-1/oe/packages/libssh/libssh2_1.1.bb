SECTION = "libs"
DESCRIPTION = "Client-side C library implementing the SSH2 protocol. See web site, we're moving off sourceforge."
DEPENDS = "openssl"
LICENSE = "LGPL"

DEFAULT_PREFERENCE="-1"

SRC_URI = "${SOURCEFORGE_MIRROR}/libssh2/libssh2-${PV}.tar.gz"

# just for sh4?
export ac_cv_func_malloc_0_nonnull=yes
export ac_cv_func_realloc_0_nonnull=yes

inherit autotools

EXTRA_OECONF = "--with-libssl-prefix=${STAGING_DIR}"

do_stage () {
	set -x
	install -d ${STAGING_INCDIR}/libssh
	for i in libssh2.h libssh2_publickey.h libssh2_sftp.h ; do
		install -m 0644 ${S}/include/$i ${STAGING_INCDIR}/
	done
	oe_libinstall -a -so -C src/.libs libssh2 ${STAGING_LIBDIR}
}

