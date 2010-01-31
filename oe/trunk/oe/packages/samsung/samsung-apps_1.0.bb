DESCRIPTION = "Samsung apps and tools"
SECTION = "base"
PRIORITY = "required"
LICENSE = "GPL"

SRC_URI = "file://samsung-start.sh \
"

do_install () {
	install -d ${D}${base_sbindir}
	install -m 0755    ${WORKDIR}/samsung-start.sh		${D}${base_sbindir}/
}


