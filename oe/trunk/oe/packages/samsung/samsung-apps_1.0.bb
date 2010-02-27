DESCRIPTION = "Samsung apps and tools"
SECTION = "base"
PRIORITY = "required"
LICENSE = "GPL"

SRC_URI = "file://samsung-start.sh \
	file://MicomCtrl.c \
"

do_configure () {
	install -m 0644 ${WORKDIR}/MicomCtrl.c ${S}/
}

do_compile () {
	${CC} ${CFLAGS} -o ${S}/MicomCtrl ${S}/MicomCtrl.c
}

do_install () {
	install -d ${D}${base_sbindir}
	install -m 0755 ${S}/MicomCtrl ${D}${base_sbindir}/
	install -m 0755 ${WORKDIR}/samsung-start.sh ${D}${base_sbindir}/
}


