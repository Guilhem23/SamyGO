require gcc-common.inc

DEPENDS = "mpfr gmp"
NATIVEDEPS = "mpfr-native gmp-native"

# SamyGO: workaround samsung download sources
FETCHCOMMAND_wget = "/usr/bin/env 'PATH=${PATH}' wget -t 5 --passive-ftp --no-check-certificate -P ${DL_DIR} \
	-O LA46B650.zip http://opensource.samsung.com/tv_n_video/la46b650/"
RESUMECOMMAND_wget = "/usr/bin/env 'PATH=${PATH}' wget -c -t 5 --passive-ftp --no-check-certificate -P ${DL_DIR} \
	-O LA46B650.zip http://opensource.samsung.com/tv_n_video/la46b650/"

SRC_URI = "http://opensource.samsung.com/tv_n_video/la46b650/LA46B650.zip \
	file://zecke-xgcc-cpp.patch;patch=1 \
	file://SamyGO-Branding.patch;patch=1 \
"

SRC_URI[md5sum] = "177f98d7a933f2de0c77b59241aaed34"
SRC_URI[sha256sum] = "154a15e19a2439e7d9a5d2cc431f4269c93fd08d1d31356be3579cf1243cc097"

do_unpack2() {
	tar --strip-components=2 -xvzf ${WORKDIR}/SELP.3.2.x-Chelsea.src.tgz SELP.3.2.x-Chelsea.src/Toolchain/gcc-4.2.0-4.0.9.tgz
	tar -xvzf gcc-4.2.0-4.0.9.tgz -C ${WORKDIR}/
	rm gcc-4.2.0-4.0.9.tgz
	rm -f ${WORKDIR}/*.zip ${WORKDIR}/SELP* ${WORKDIR}/*.tgz ${WORKDIR}/*.gz ${WORKDIR}/README.txt || true
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
