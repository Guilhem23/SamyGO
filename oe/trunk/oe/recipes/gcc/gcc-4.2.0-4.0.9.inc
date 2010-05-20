require gcc-common.inc

DEPENDS = "mpfr gmp"
NATIVEDEPS = "mpfr-native gmp-native"

SRC_URI = "${SAMSUNG_OS_MIRROR}/32B650.zip \
	file://zecke-xgcc-cpp.patch;patch=1 \
	file://SamyGO-Branding.patch;patch=1 \
"

SRC_URI[md5sum] = "232c2397d511253a8d6dcf66fa8d9bc2"
SRC_URI[sha256sum] = "78cecf54357d014450355c35af21d2ad330c782a89f56fead6a65b238dbff0f6"

do_unpack2() {
	tar --strip-components=2 -xvzf ${WORKDIR}/SELP.3.2.x-Chelsea.src.tgz SELP.3.2.x-Chelsea.src/Toolchain/gcc-4.2.0-4.0.9.tgz
	tar -xvzf gcc-4.2.0-4.0.9.tgz -C ${WORKDIR}/
	rm gcc-4.2.0-4.0.9.tgz
	rm -f ${WORKDIR}/*.zip ${WORKDIR}/SELP* ${WORKDIR}/*.tgz ${WORKDIR}/*.gz || true
}

addtask unpack2 before do_patch after do_unpack

# Language Overrides
FORTRAN = ""
JAVA = ""

EXTRA_OECONF_BASE = "--disable-libssp --disable-bootstrap --disable-libgomp --disable-libmudflap"
EXTRA_OECONF_INITIAL = "--disable-libmudflap --disable-libgomp --disable-libssp"
EXTRA_OECONF_INTERMEDIATE = "--disable-libmudflap --disable-libgomp --disable-libssp"

ARM_INSTRUCTION_SET = "arm"

BINV = "4.2"
