DESCRIPTION = "native yes command replacement"
LICENSE = "GPL"
SECTION = "base"
PRIORITY = "required"
SRC_URI = "file://yes.c"
S = "${WORKDIR}/yes-native-${PV}"
PR = "r2"

FILESDIR = "${@os.path.dirname(bb.data.getVar('FILE',d,1))}/yes-${PV}"

inherit native 

do_configure() {
	install -m 0644 ${WORKDIR}/yes.c ${S}/
}

do_compile() {
	${CC} ${CFLAGS} -o ${S}/yes ${S}/yes.c
}

do_stage() {
	install -d ${STAGING_BINDIR}/
        install -m 0755 ${S}/yes ${STAGING_BINDIR}/
}
