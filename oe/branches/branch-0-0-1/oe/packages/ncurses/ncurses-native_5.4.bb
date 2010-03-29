SECTION = "libs"
require ncurses_${PV}.bb
inherit native
EXTRA_OEMAKE = '"BUILD_CCFLAGS=${BUILD_CCFLAGS}"  "LIBTOOL=${HOST_SYS}-libtool"'
EXTRA_OECONF_append_samygo += " --with-libtool"
DEPENDS = ""
FILESPATH = "${FILE_DIRNAME}/local:${FILE_DIRNAME}/ncurses-${PV}-${PR}:${FILE_DIRNAME}/ncurses-${PV}:${FILE_DIRNAME}/ncurses:${FILE_DIRNAME}"
