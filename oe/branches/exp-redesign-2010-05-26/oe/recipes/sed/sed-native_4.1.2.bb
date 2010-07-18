SECTION = "console/utils"
require sed_${PV}.bb
inherit native

DEPENDS = "gettext-native"

#SamyGO: do_install to prevent common alternative install; added DEPENDS: gettext-native
do_install () {
	autotools_do_install
}
