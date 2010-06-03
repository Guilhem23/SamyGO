SECTION = "devel"
DESCRIPTION = "Sanitized set of 2.6 kernel headers for the C library's use."
# This package is derived from the original linux-libc-headers at
#   http://ep09.pld-linux.org/~mmazur/linux-libc-headers/
# More specifically, llh-2.6.12.0 was patched up to 2.6.16-rc6 with 
# the official linux patches (where applicable) and then fixed up just
# enough to build glibc-2.4. BEWARE!
#
# license note from the linux-libc-headers package: 
#   Linux-libc-headers are derived from linux kernel headers. For license of a
#   particular header, check it's content, and if copyright notice isn't present,
#   standard linux kernel license applies. 
# since we assume GPL for linux i think we can also assume it here
LICENSE = "GPL"
DEFAULT_PREFERENCE = "-1"
INHIBIT_DEFAULT_DEPS = "1"
PR = "r011"
RDEPENDS_${PN}-dev = ""
RRECOMMENDS_${PN}-dbg = "${PN}-dev (= ${EXTENDPV})"

# SamyGO: workaround samsung download sources
FETCHCOMMAND_wget = "/usr/bin/env 'PATH=${PATH}' wget -t 5 --passive-ftp --no-check-certificate -P ${DL_DIR} \
	-O LA46B650.zip http://opensource.samsung.com/tv_n_video/la46b650/"
RESUMECOMMAND_wget = "/usr/bin/env 'PATH=${PATH}' wget -c -t 5 --passive-ftp --no-check-certificate -P ${DL_DIR} \
	-O LA46B650.zip http://opensource.samsung.com/tv_n_video/la46b650/"

SRC_URI = "http://opensource.samsung.com/tv_n_video/la46b650/LA46B650.zip \
	file://procinfo.h \
	file://arm-syscall-define.patch;patch=1 \
	file://defconfig"

SRC_URI[md5sum] = "177f98d7a933f2de0c77b59241aaed34"
SRC_URI[sha256sum] = "154a15e19a2439e7d9a5d2cc431f4269c93fd08d1d31356be3579cf1243cc097"

S = "${WORKDIR}/linux/linux-${PR}"

do_unpack2() {
	tar -xvzf ${WORKDIR}/linux.cip.open.tgz -C ${WORKDIR}/
	rm -f ${WORKDIR}/*.zip ${WORKDIR}/SELP* ${WORKDIR}/*.tgz ${WORKDIR}/*.gz ${WORKDIR}/README.txt || true
}

addtask unpack2 before do_patch after do_unpack

do_configure () {
	echo > .mvl_cross_compile
	echo ${TARGET_ARCH} > .mvl_target_cpu
	cp ${WORKDIR}/defconfig .config
	make include/linux/version.h
	case ${TARGET_ARCH} in
		alpha*)   ARCH=alpha ;;
		arm*)     ARCH=arm ;;
		cris*)    ARCH=cris ;;
		hppa*)    ARCH=parisc ;;
		i*86*)    ARCH=i386 ;;
		ia64*)    ARCH=ia64 ;;
		mips*)    ARCH=mips ;;
		m68k*)    ARCH=m68k ;;
		powerpc*) ARCH=ppc ;;
		s390*)    ARCH=s390 ;;
		sh*)      ARCH=sh ;;
		sparc64*) ARCH=sparc64 ;;
		sparc*)   ARCH=sparc ;;
		x86_64*)  ARCH=x86_64 ;;
	esac

	echo $(pwd)

	echo "all:" > Makefile
	rm -f ".co*" "*"
	rm -rf "[^i]*" "init" "ipc"
}

do_stage () {
	install -d ${STAGING_INCDIR}
	rm -rf ${STAGING_INCDIR}/linux ${STAGING_INCDIR}/asm ${STAGING_INCDIR}/asm-generic
	if [ "$ARCH" = "arm" ]; then
		cp ${WORKDIR}/procinfo.h include/asm/
	fi
	cp -pfLR include/linux ${STAGING_INCDIR}/
	cp -pfLR include/asm ${STAGING_INCDIR}/
	cp -pfLR include/asm-generic ${STAGING_INCDIR}/
#	cp -pfLR include/mtd ${STAGING_INCDIR}/
	rm -rf ${CROSS_DIR}/${TARGET_SYS}/include/linux
	rm -rf ${CROSS_DIR}/${TARGET_SYS}/include/asm
	rm -rf ${CROSS_DIR}/${TARGET_SYS}/include/asm-generic
	install -d ${CROSS_DIR}/${TARGET_SYS}/include
	cp -pfLR include/linux ${CROSS_DIR}/${TARGET_SYS}/include/
	cp -pfLR include/asm ${CROSS_DIR}/${TARGET_SYS}/include/
	cp -pfLR include/asm-generic ${CROSS_DIR}/${TARGET_SYS}/include/
#	cp -pfLR include/mtd ${CROSS_DIR}/${TARGET_SYS}/include/
}

do_install() {
	install -d ${D}${includedir}
	cp -pfLR include/linux ${D}${includedir}/
	cp -pfLR include/asm ${D}${includedir}/
	cp -pfLR include/asm-generic ${D}${includedir}/
}

do_install_append_arm() {
	cp ${WORKDIR}/procinfo.h ${D}${includedir}/asm/
}
