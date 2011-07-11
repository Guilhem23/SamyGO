DESCRIPTION = "steal micom -> exeDSP communication"
LICENSE = "GPL"

inherit autotools

PR = "r02"

SRC_URI = "file://sertest.c \
"

S = "${WORKDIR}"

do_compile(){
	${CC} -o ${PN} ${PN}.c
}

do_install(){
        install -d ${D}${bindir}
        install -m 0755 ${PN} ${D}${bindir}
}

