SECTION = "devel"
# cut-down gcc for kernel builds
# only installs ${TARGET_PREFIX}gcc-${PV}, not ${TARGET_PREFIX}gcc.

require gcc-cross-initial_${PV}.bb

PROVIDES = "virtual/${TARGET_PREFIX}gcc-${PV}"

# Need for kernel-build
EXTRA_OECONF_append_sh4 = "--with-multilib-list=m4,m4-nofpu --with-cpu=sh4 "

do_install () {
	:
}

do_stage () {
	cd gcc
	oe_runmake install-common install-headers install-libgcc
	install -m 0755 xgcc ${CROSS_DIR}/bin/${TARGET_PREFIX}gcc-${PV}
}
