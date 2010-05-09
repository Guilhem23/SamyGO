SECTION = "console/utils"
require sed_${PV}.bb
inherit native

#SamyGO:
do_install () {
	autotools_do_install
	install -d ${D}${base_bindir}
	
}

