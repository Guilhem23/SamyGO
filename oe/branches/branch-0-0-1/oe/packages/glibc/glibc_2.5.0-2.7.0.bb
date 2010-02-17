require glibc.inc

ARM_INSTRUCTION_SET = "arm"

PACKAGES_DYNAMIC = "libc6*"
RPROVIDES_${PN}-dev = "libc6-dev"

PR = "r1"

# the -isystem in bitbake.conf screws up glibc do_stage
BUILD_CPPFLAGS = "-I${STAGING_DIR}/${BUILD_SYS}/include"
TARGET_CPPFLAGS = "-I${STAGING_DIR}/${TARGET_SYS}/include"


FILESDIR = "${@os.path.dirname(bb.data.getVar('FILE',d,1))}/glibc-${PV}"

GLIBC_ADDONS ?= "nptl"

GLIBC_BROKEN_LOCALES ?= "sid_ET tr_TR mn_MN gez_ET gez_ER bn_BD te_IN"

#
# For now, we will skip building of a gcc package if it is a uclibc one
# and our build is not a uclibc one, and we skip a glibc one if our build
# is a uclibc build.
#
# See the note in gcc/gcc_3.4.0.oe
#

python __anonymous () {
    import bb, re
    uc_os = (re.match('.*uclibc$', bb.data.getVar('TARGET_OS', d, 1)) != None)
    if uc_os:
        raise bb.parse.SkipPackage("incompatible with target %s" %
                                   bb.data.getVar('TARGET_OS', d, 1))
}

RDEPENDS_${PN}-dev = "linux-libc-headers-dev"

SRC_URI = "http://www.samsung.com/global/opensource/files/LE46A956.zip \
	file://zecke-sane-readelf.patch;patch=1 \
           file://etc/ld.so.conf \
           file://generate-supported.mk \
"

# SRC_URI_append_samygo = " file://SamyGO-disable-localedata.patch;patch=1 "
SRC_URI_append_samygo = " file://selp_atan-weak.patch;patch=1 "

S = "${WORKDIR}/GLIBC/glibc-2.5"
B = "${WORKDIR}/GLIBC/__build"
B = "${WORKDIR}/GLIBC/glibc-2.5/BUILD"

EXTRA_OECONF = "--enable-kernel=${OLDEST_KERNEL} --enable-shared \
		--with-tls --with-__thread \
                --without-cvs --disable-profile --disable-debug --without-gd \
                --enable-add-ons=${GLIBC_ADDONS} \
                --with-headers=${STAGING_INCDIR} \
                ${GLIBC_EXTRA_OECONF}"

# real reason is to disable -fomit-frame-pointer ? * Arris *
# TARGET_CFLAGS_append_sh4 = " -fno-schedule-insns "
# CFLAGS_append_sh4 = " -fno-schedule-insns "

do_unpack2() {
        tar -xvzf ${WORKDIR}/selp_glibc_2.7.tar.gz -C ${WORKDIR}/
        rm -f ${WORKDIR}/*.gz ${WORKDIR}/Re* ${WORKDIR}/*.bz2 || true
}

addtask unpack2 before do_munge after do_unpack

do_munge() {
	:
}

addtask munge before do_patch after do_unpack


do_configure () {
	CPPFLAGS="" CFLAGS="-g -O2 -fno-omit-frame-pointer" oe_runconf
}

rpcsvc = "bootparam_prot.x nlm_prot.x rstat.x \
	  yppasswd.x klm_prot.x rex.x sm_inter.x mount.x \
	  rusers.x spray.x nfs_prot.x rquota.x key_prot.x"

do_compile () {
        unset LDFLAGS
	unset CFLAGS
        base_do_compile
}

do_stage() {
	rm -f ${STAGING_LIBDIR}/libc.so.6
	oe_runmake 'install_root=${STAGING_DIR}/${HOST_SYS}' \
		   'includedir=/include' 'libdir=/lib' 'slibdir=/lib' \
		   '${STAGING_LIBDIR}/libc.so.6' \
		   install-headers install-lib

	install -d ${STAGING_INCDIR}/gnu \
		   ${STAGING_INCDIR}/bits \
		   ${STAGING_INCDIR}/rpcsvc
	install -m 0644 ${S}/include/gnu/stubs.h ${STAGING_INCDIR}/gnu/
	install -m 0644 ${B}/bits/stdio_lim.h ${STAGING_INCDIR}/bits/
	install -m 0644 misc/syscall-list.h ${STAGING_INCDIR}/bits/syscall.h
#	for r in ${rpcsvc}; do
#		h=`echo $r|sed -e's,\.x$,.h,'`
#		install -m 0644 ${S}/sunrpc/rpcsvc/$h ${STAGING_INCDIR}/rpcsvc/
#	done
	for i in libc.a libc_pic.a libc_nonshared.a; do
		install -m 0644 ${B}/$i ${STAGING_LIBDIR}/ || die "failed to install $i"
	done
	echo 'GROUP ( libpthread.so.0 libpthread_nonshared.a )' > ${STAGING_LIBDIR}/libpthread.so
	echo 'GROUP ( libc.so.6 libc_nonshared.a )' > ${STAGING_LIBDIR}/libc.so

	rm -f ${CROSS_DIR}/${TARGET_SYS}/lib/libc.so.6
	oe_runmake 'install_root=${CROSS_DIR}/${TARGET_SYS}' \
		   'includedir=/include' 'libdir=/lib' 'slibdir=/lib' \
		   '${CROSS_DIR}/${TARGET_SYS}/lib/libc.so.6' \
		   install-headers install-lib

	install -d ${CROSS_DIR}/${TARGET_SYS}/include/gnu \
		   ${CROSS_DIR}/${TARGET_SYS}/include/bits \
		   ${CROSS_DIR}/${TARGET_SYS}/include/rpcsvc
	install -m 0644 ${S}/include/gnu/stubs.h ${CROSS_DIR}/${TARGET_SYS}/include/gnu/
	install -m 0644 ${B}/bits/stdio_lim.h ${CROSS_DIR}/${TARGET_SYS}/include/bits/
	install -m 0644 misc/syscall-list.h ${CROSS_DIR}/${TARGET_SYS}/include/bits/syscall.h
#	for r in ${rpcsvc}; do
#		h=`echo $r|sed -e's,\.x$,.h,'`
#		install -m 0644 ${S}/sunrpc/rpcsvc/$h ${CROSS_DIR}/${TARGET_SYS}/include/rpcsvc/
#	done

	for i in libc.a libc_pic.a libc_nonshared.a; do
		install -m 0644 ${B}/$i ${CROSS_DIR}/${TARGET_SYS}/lib/ || die "failed to install $i"
	done
	echo 'GROUP ( libpthread.so.0 libpthread_nonshared.a )' > ${CROSS_DIR}/${TARGET_SYS}/lib/libpthread.so
	echo 'GROUP ( libc.so.6 libc_nonshared.a )' > ${CROSS_DIR}/${TARGET_SYS}/lib/libc.so
}

require glibc-package.bbclass
