require gcc-common.inc

DEPENDS = "mpfr gmp"
NATIVEDEPS = "mpfr-native gmp-native"

S = "${WORKDIR}/gcc-4.2.0"

SRC_URI = "${GNU_MIRROR}/gcc/gcc-4.2.0/gcc-4.2.0.tar.bz2 \
	file://300-libstdc++-pic.patch;patch=1 \
	file://304-index_macro.patch;patch=1 \
	file://305-libmudflap-susv3-legacy.patch;patch=1 \
	file://307-locale_facets.patch;patch=1 \
	file://402-libbackend_dep_gcov-iov.h.patch;patch=1 \
	file://602-sdk-libstdc++-includes.patch;patch=1 \
	file://800-arm-bigendian.patch;patch=1 \
	file://904-flatten-switch-stmt-00.patch;patch=1 \
	file://arm-nolibfloat.patch;patch=1 \
	file://arm-softfloat.patch;patch=1 \
	file://gcc41-configure.in.patch;patch=1 \
	file://arm-thumb.patch;patch=1 \
	file://arm-thumb-cache.patch;patch=1 \
	file://zecke-xgcc-cpp.patch;patch=1 \
	file://cache-amnesia.patch;patch=1 \
	file://pr34130.patch;patch=1 \
"

SRC_URI[md5sum] = "4798c23548dc791ea12a139e18940684"
SRC_URI[sha256sum] = "7b6c56ec37e52d24745a07bcf37e416b22141b0d0f1e80f71f9b1fe375e83268"

# Language Overrides
FORTRAN = ""
JAVA = ""

EXTRA_OECONF_BASE = "--disable-libssp --disable-bootstrap --disable-libgomp --disable-libmudflap"
EXTRA_OECONF_INITIAL = "--disable-libmudflap --disable-libgomp --disable-libssp"
EXTRA_OECONF_INTERMEDIATE = "--disable-libmudflap --disable-libgomp --disable-libssp"

ARM_INSTRUCTION_SET = "arm"

BINV = "4.2"
