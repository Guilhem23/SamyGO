require gcc-common.inc

DEPENDS = "mpfr gmp"

SRC_URI = "http://www.samsung.com/global/opensource/files/32B650.zip \
	file://zecke-xgcc-cpp.patch;patch=1 \
	file://SamyGO-Branding.patch;patch=1 \
"

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

ARM_INSTRUCTION_SET = "arm"

BINV = "4.2"
