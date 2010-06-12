require linux-libc-headers.inc

INHIBIT_DEFAULT_DEPS = "1"
DEPENDS += "unifdef-native"
PR = "r4"

SRC_URI = "${KERNELORG_MIRROR}/pub/linux/kernel/v2.6/linux-2.6.18.8.tar.bz2 \
           file://arm-syscall-define.patch;patch=1"

S = "${WORKDIR}/linux-2.6.18.8"

SRC_URI[md5sum] = "dce47badc1faf34b355a10b97ae5d391"
SRC_URI[sha256sum] = "945b3014f8048cd87fdff90014afa4ff241f134bceafbfdbd42dba1be8df2ba8"

set_arch() {
	case ${TARGET_ARCH} in
		alpha*)   ARCH=alpha ;;
		arm*)     ARCH=arm ;;
		cris*)    ARCH=cris ;;
		hppa*)    ARCH=parisc ;;
		i*86*)    ARCH=i386 ;;
		ia64*)    ARCH=ia64 ;;
		mips*)    ARCH=mips ;;
		m68k*)    ARCH=m68k ;;
		powerpc*) ARCH=powerpc ;;
		s390*)    ARCH=s390 ;;
		sh*)      ARCH=sh ;;
		sparc64*) ARCH=sparc64 ;;
		sparc*)   ARCH=sparc ;;
		x86_64*)  ARCH=x86_64 ;;
	esac
}

do_configure() {
	set_arch
	oe_runmake allnoconfig ARCH=${ARCH}
}

do_compile () {
}

do_install() {
	set_arch
	oe_runmake headers_install INSTALL_HDR_PATH=${D}${exec_prefix} ARCH=${ARCH}
	# Add UTS_RELEASE to version.h. UTS_RELEASE was moved from version.h to
	# utsrelease.h in order to avoid recompiling a kernel every time a localversion
	# changed. Since the our headers are static and we're not compiling an
	# actual kernel, re-adding UTS_RELEASE does't hurt, and it allows uclibc to
	# compile with kernel headers that work with EABI on ARM
	echo '#define UTS_RELEASE "2.6.18.8"' >> ${STAGING_INCDIR}/linux/version.h
}

do_install_append_arm() {
	cp include/asm-arm/procinfo.h ${D}${includedir}/asm
}
