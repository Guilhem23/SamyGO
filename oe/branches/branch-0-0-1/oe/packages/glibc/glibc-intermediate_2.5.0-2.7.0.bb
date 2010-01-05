# BUILD_SYS = "arm-linux-gnueabi"
require glibc_${PV}.bb

do_install () {
	:
}

# gcc uses -Werror which break on a "you have no thumb interwork" _warning_
do_configure_prepend() {
	sed -i s:-Werror:: ${S}/configure
}



PACKAGES = ""
PACKAGES_DYNAMIC = ""
PROVIDES = "virtual/${TARGET_PREFIX}libc-for-gcc"
DEPENDS = "virtual/${TARGET_PREFIX}gcc-initial linux-libc-headers"
GLIBC_ADDONS = "nptl,ports"
GLIBC_EXTRA_OECONF = ""
