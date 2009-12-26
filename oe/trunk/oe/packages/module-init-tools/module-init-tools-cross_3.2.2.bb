LICENSE = "GPL"
require module-init-tools_${PV}.bb
inherit cross
DEFAULT_PREFERENCE = "0"
PROVIDES += "virtual/${TARGET_PREFIX}depmod virtual/${TARGET_PREFIX}depmod-2.6"

SRC_URI += "file://fix_build_native_nonlinux.patch;patch=1"

EXTRA_OECONF_append = " --program-prefix=${TARGET_PREFIX}"

do_stage () {
        oe_runmake install
        mv ${bindir}/${TARGET_PREFIX}depmod ${bindir}/${TARGET_PREFIX}depmod-2.6
}

do_install () {
        :
}
