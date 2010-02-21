DESCRIPTION = "WiseLink video player aspect ratio fix for exeDSP"
SECTION = "category"
MAINTAINER = "Tomas Vanek <use@samygo.forum>"
HOMEPAGE = "http://samygo.sourceforge.net"

PR = "r1"

DEPENDS = "elfpatcher"
RDEPENDS = "elfpatcher"

SRC_URI = " \
	file://${PN}.c \
	file://${LDSCRIPT} \
	file://README \
"

COMPATIBLE_HOST = "sh4-linux"
COMPATIBLE_MACHINE = "T-RBYDEUC"
#COMPATIBLE_MACHINE = "(T-RBYDEUC|T-AMBDFRC)"
PACKAGE_ARCH = "${MACHINE_ARCH}"

PACKAGES = "${PN}"

S = ${WORKDIR}

FW_VER = ${@re.sub('\+Samsung', '', bb.data.getVar('PREFERRED_VERSION_samsung-firmware', d, 1))}
FIRMWARE = ${MACHINE}-${FW_VER}

# T-RBYDEUC exeDSP has symbols stripped, so we need one ld script
# with right addresses for each firmware version
LDSCRIPT = ${FIRMWARE}-arfix.ld

PNF = "${FIRMWARE}-arfix_${PV}"
ELF = "${PNF}.elfpatch"

EXTRA_CFLAGS += "-Os -m4 -Wall -mnomacsave -fshort-wchar -nostartfiles -nostdlib"

do_compile() {
        ${CC} ${CFLAGS} ${EXTRA_CFLAGS} -o ${ELF} ${PN}.c -Wl,-T${LDSCRIPT},-n
	${STRIP} --remove-section=.comment ${ELF}
	chmod -x ${ELF}
}

do_install() {
	echo '#! /bin/sh' > arfix
	echo >> arfix
	echo 'elfpatcher -p $(pidof exeDSP)' /usr/share/elfpatches/${ELF} >> arfix
	mkdir -p ${D}/etc/init.d
	install -m 0755 arfix ${D}/etc/init.d/
	mkdir -p ${D}/usr/share/elfpatches
	install -m 0644 ${ELF} ${D}/usr/share/elfpatches/
	cp README ${D}/README-arfix
	do_simple_install
}

DT = ${WORKDIR}/${PNF}

do_simple_install() {
	echo '#! /bin/sh' > arfix.sh
	echo >> arfix.sh
	echo './elfpatcher -p $(pidof exeDSP)' ${ELF} >> arfix.sh
	mkdir -p ${DT}
	install -m 0755 arfix.sh ${DT}/
	install -m 0755 ${STAGING_DIR}/sh4-linux/bin/elfpatcher ${DT}/
	install -m 0644 ${ELF} ${DT}/
	cp README ${DT}/
	zip -r -b ${WORKDIR} ${DEPLOY_DIR}/SamyGO-${PNF}.zip ${PNF}
#	mkdir -p ${DEPLOY_DIR_TAR}
#	/bin/tar -c -v -f ${DEPLOY_DIR_TAR}/${PNF}.tar -C ${WORKDIR} ${PNF}
#	/bin/gzip -f ${DEPLOY_DIR_TAR}/${PNF}.tar
}

FILES_${PN} = "/etc/init.d /usr/share/elfpatches"
