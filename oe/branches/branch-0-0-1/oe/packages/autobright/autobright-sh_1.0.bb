DESCRIPTION = "libGame.so launcher and .so lib injector for exeDSP"
SECTION = "category"
MAINTAINER = "Tomas Vanek <use@samygo.forum>"
HOMEPAGE = "http://samygo.sourceforge.net"

PR = "r1"

# for install with SDL stuff
DEPENDS_SDL = "SDL-Samsung freetype libsdl-ttf libsdl-gfx"

DEPENDS = "launcher-sh ${DEPENDS_SDL}"
RDEPENDS = ""

SRC_URI = "\
	file://${FILE_DIRNAME}/files/* \
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

CFLAGS_append = " -I${STAGING_DIR}/${PACKAGE_ARCH}-${TARGET_OS}/exedsp/include/"
CFLAGS_append = " -I${STAGING_DIR}/${PACKAGE_ARCH}-${TARGET_OS}/exedsp/${FIRMWARE}/include/"
LDFLAGS_append = " -L${STAGING_DIR}/${PACKAGE_ARCH}-${TARGET_OS}/exedsp/lib/"

# T-RBYDEUC exeDSP has symbols stripped, so we need one ld script
# with right addresses for each firmware version

PNF = "${PN}_${PV}_${FIRMWARE}"

do_install() {
	do_simple_install
}

DT = ${WORKDIR}/${PNF}
DLIB = ${DT}/SEC_GAME/${PN}/

do_simple_install() {
	mkdir -p ${DLIB}
	install -m 0644 Autobright.dat ${DLIB}
	install -m 0644 Autobright.jpg ${DLIB}
	install -m 0644 libAutobright.so ${DLIB}
	install -m 0644 autobright.so ${DLIB}
	cd ${DT}
	zip -r -b ${WORKDIR} ${DEPLOY_DIR}/SamyGO-${PNF}.zip SEC_GAME
}

