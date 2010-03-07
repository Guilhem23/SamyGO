DESCRIPTION = "memread - read physical memory in a running Linux system"
HOMEPAGE = "http://ar7-firmware.berlios.de"
SECTION = "console/system"
LICENSE = "GPL"

PV = "0.1.0"
PR = "r01"

INHIBIT_PACKAGE_STRIP = "1"

PACKAGES = "${PN}-dbg ${PN} ${PN}-bin ${PN}-dev ${PN}-doc ${PN}-locale"

SRC_URI = "http://ar7-firmware.berlios.de/tools/${PN}.c \
"

S = ${WORKDIR}

CFLAGS += " -static"

do_configure() {
	:
}

do_compile () {
	${CC} ${CFLAGS} -o ${PN} ${PN}.c 
}

do_install() {
	install -d ${D}/bin
        install -m 0755 ${PN} ${D}/bin/
}

FILES_${PN} += "/bin"
