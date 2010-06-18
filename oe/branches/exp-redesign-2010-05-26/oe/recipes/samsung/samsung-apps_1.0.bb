DESCRIPTION = "Samsung apps and tools"
SECTION = "base"
PRIORITY = "required"
LICENSE = "GPL"

SRC_URI = "file://samsung-install.sh \
	file://MicomCtrl.c \
"

do_configure () {
	install -m 0644 ${WORKDIR}/MicomCtrl.c ${S}/
}

do_compile () {
	$CC $CFLAGS $LDFLAGS -o ${S}/micomctrl ${S}/MicomCtrl.c
}

do_install () {
	install -d ${D}${base_sbindir}
	install -m 0755 ${S}/micomctrl ${D}${base_sbindir}/
	install -m 0755 ${WORKDIR}/samsung-install.sh ${D}${base_sbindir}/
}

