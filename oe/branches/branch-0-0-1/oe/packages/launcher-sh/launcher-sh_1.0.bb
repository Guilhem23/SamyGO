DESCRIPTION = "libGame.so launcher and .so lib injector for exeDSP"
SECTION = "category"
MAINTAINER = "Tomas Vanek <use@samygo.forum>"
HOMEPAGE = "http://samygo.sourceforge.net"

PR = "r1"

# for install with SDL stuff
DEPENDS_SDL = "SDL-Samsung freetype libsdl-ttf"

DEPENDS = "elfpatcher glibc ${DEPENDS_SDL}"
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
DLIB = ${DT}/mtd_exe/

do_simple_install() {
	mkdir -p ${DT}
	install -m 0755 install-launcher.sh ${DT}/
	install -m 0755 ${STAGING_DIR}/sh4-linux/bin/elfpatcher ${DT}/
	install -m 0644 inj-dbgin.elfpatch ${DT}/
	mkdir -p ${DLIB}
	install -m 0644 libc_stub.so ${DLIB}
	install -m 0644 libpthread_stub.so ${DLIB}
	install -m 0644 sym.so ${DLIB}
	install -m 0644 inj.so ${DLIB}
	install -m 0644 launcher.so ${DLIB}
	do_sdl_install
	mkdir -p ${DT}/mtd_rwarea
	install -m 0644 libso.autoload ${DT}/mtd_rwarea/
#	cp README ${DT}/
	zip -r -b ${WORKDIR} ${DEPLOY_DIR}/SamyGO-${PNF}.zip ${PNF}
#	mkdir -p ${DEPLOY_DIR_TAR}
#	/bin/tar -c -v -f ${DEPLOY_DIR_TAR}/${PNF}.tar -C ${WORKDIR} ${PNF}
#	/bin/gzip -f ${DEPLOY_DIR_TAR}/${PNF}.tar
}

#WL = ${TMPDIR}/work/sh4-linux

strip_and_install_lib() {
	destname=${2:-${1}}
	if test "${3}"
	then
		oefatal "too many parameters strip_and_install_lib $1 $2 $3" > /dev/tty
	fi		
	${STRIP} -o ${WORKDIR}/${destname} ${STAGING_LIBDIR}/${1}
	install -m 0644 ${WORKDIR}/${destname} ${DLIB}
}
do_sdl_install() {
# unfortunately /mtd_exe has RFS filesystem so no symbolic links
	strip_and_install_lib libpthread-2.5.so libpthread.so.0
	strip_and_install_lib libSDL.so
	strip_and_install_lib libfreetype.so.6.4.0 libfreetype.so.6
	strip_and_install_lib libSDL_ttf-2.0.so.0.* libSDL_ttf-2.0.so.0
}

#FILES_${PN} = "/etc/init.d /usr/share/elfpatches"
