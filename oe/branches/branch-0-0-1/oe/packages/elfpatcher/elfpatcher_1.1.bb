DESCRIPTION = "elfpatcher is a tool for writing segments from elf file \
to another elf or to process memory using PTRACE"
LICENSE = "GPL3"
SECTION = "category"
MAINTAINER = "Tomas Vanek <use@samygo.forum>"
HOMEPAGE = "http://samygo.sourceforge.net"

PR = "r01"

DEPENDS = "libelf"
RDEPENDS = ""

SRC_URI = " \
	file://${PN}.c \
"

PACKAGES = "${PN}"

S = ${WORKDIR}

EXTRA_CFLAGS += " -Os"

do_compile(){
        ${CC} ${CFLAGS} ${EXTRA_CFLAGS} ${LDFLAGS} ${EXTRA_LDFLAGS} -o ${PN} ${PN}.c -Wl,-Bstatic,-lelf,-Bdynamic
	${STRIP} ${PN}
}

do_install() {
        mkdir -p ${D}/${bindir}
        install -m 0755 ${PN} ${D}/${bindir}/
}

do_populate_staging(){
	install -m 0755 ${PN} ${STAGING_DIR}/${PACKAGE_ARCH}-${TARGET_OS}/bin/
}

