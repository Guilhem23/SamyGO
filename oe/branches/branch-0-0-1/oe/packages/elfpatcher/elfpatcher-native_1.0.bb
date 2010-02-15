PN_ORIG = elfpatcher
require ${PN_ORIG}_${PV}.bb
inherit native

DEPENDS = "libelf-native"
RDEPENDS = ""

SRC_URI = " \
	file://${PN_ORIG}.c \
"

do_compile(){
        ${CC} ${CFLAGS} ${EXTRA_CFLAGS} ${LDFLAGS} ${EXTRA_LDFLAGS} -o ${PN_ORIG} ${PN_ORIG}.c -lelf
}

do_populate_staging(){
	install -m 0755 ${PN_ORIG} ${bindir}
}

