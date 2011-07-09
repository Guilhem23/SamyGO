DESCRIPTION = "libGame.so launcher and .so lib injector for exeDSP"
SECTION = "category"
MAINTAINER = "Tomas Vanek <use@samygo.forum>"
HOMEPAGE = "http://samygo.sourceforge.net"

PR = "r1"

DEPENDS = "elfpatcher glibc"
RDEPENDS = "elfpatcher"

SRC_URI = "\
	file://${FILE_DIRNAME}/files/* \
	file://${FILE_DIRNAME}/${FIRMWARE}-files/* \
"

COMPATIBLE_HOST = "sh4-linux"
COMPATIBLE_MACHINE = "T-RBYDEUC"
PACKAGE_ARCH = "${MACHINE_ARCH}"

PACKAGES = "${PN}"

S = ${WORKDIR}

python __anonymous () {
    import bb, re
    fw_ver = re.sub('\+Samsung', '', bb.data.getVar('PREFERRED_VERSION_samsung-firmware', d, 1))
    bb.data.setVar('FW_VER', fw_ver, d)
    fw_part = fw_ver.split(".")
}

FIRMWARE = ${MACHINE}-${FW_VER}

# T-RBYDEUC exeDSP has symbols stripped, so we need one ld script
# with right addresses for each firmware version

PNF = "${PN}_${PV}_${FIRMWARE}"

do_install() {
#	mkdir -p ${D}/usr/share/elfpatches
#	install -m 0644 ${ELF} ${D}/usr/share/elfpatches/
#	cp README ${D}/usr/share/elfpatches/README-inj-dbgin
	do_simple_install
}

DT = ${WORKDIR}/${PNF}

do_simple_install() {
	mkdir -p ${DT}
	install -m 0755 install-launcher.sh ${DT}/
	install -m 0755 ${STAGING_DIR}/sh4-linux/bin/elfpatcher ${DT}/
	install -m 0644 inj-dbgin.elfpatch ${DT}/
	mkdir -p ${DT}/mtd_exe
	install -m 0644 libc_stub.so ${DT}/mtd_exe/
	install -m 0644 libpthread_stub.so ${DT}/mtd_exe/
	install -m 0644 sym.so ${DT}/mtd_exe/
	install -m 0644 inj.so ${DT}/mtd_exe/
	install -m 0644 launcher.so ${DT}/mtd_exe/
	mkdir -p ${DT}/mtd_rwarea
	install -m 0644 libso.autoload ${DT}/mtd_rwarea/
#	cp README ${DT}/
	zip -r -b ${WORKDIR} ${DEPLOY_DIR}/SamyGO-${PNF}.zip ${PNF}
#	mkdir -p ${DEPLOY_DIR_TAR}
#	/bin/tar -c -v -f ${DEPLOY_DIR_TAR}/${PNF}.tar -C ${WORKDIR} ${PNF}
#	/bin/gzip -f ${DEPLOY_DIR_TAR}/${PNF}.tar
}

#FILES_${PN} = "/etc/init.d /usr/share/elfpatches"
