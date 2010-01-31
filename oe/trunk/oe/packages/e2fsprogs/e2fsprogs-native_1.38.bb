SECTION = "base"
require e2fsprogs.inc
inherit native

EXTRA_OECONF = ""

FILESPATH = "${@base_set_filespath([ '${FILE_DIRNAME}/e2fsprogs-${PV}', '${FILE_DIRNAME}/e2fsprogs', '${FILE_DIRNAME}/files', '${FILE_DIRNAME}' ], d)}"
PACKAGES = ""
DEPENDS = ""

do_stage () {
        oe_runmake install
}