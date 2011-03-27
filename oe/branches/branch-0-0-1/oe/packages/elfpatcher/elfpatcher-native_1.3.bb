PN_ORIG = elfpatcher
require ${PN_ORIG}_${PV}.bb
inherit native

# will be usable only if host machine word size equals to sh4 word size (32-bit)

DEPENDS = "libelf-native"
RDEPENDS = ""

SRC_URI = " \
	file://${PN_ORIG}.c \
"

EXTRA_CFLAGS += -DPYTHON_PATCH -DREVERSE_LD

do_compile(){
        ${CC} ${CFLAGS} ${EXTRA_CFLAGS} ${LDFLAGS} ${EXTRA_LDFLAGS} -o ${PN_ORIG} ${PN_ORIG}.c -lelf
}

do_populate_staging(){
	install -m 0755 ${PN_ORIG} ${bindir}
}

