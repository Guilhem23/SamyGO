PR = "r1"
DESCRIPTION = "The GNU cc and gcc C compilers."
HOMEPAGE = "http://www.gnu.org/software/gcc/"
SECTION = "devel"
LICENSE = "GPL"

inherit autotools gettext

BINV = "4.2.0"

require gcc-package.inc

SRC_URI = "http://www.samsung.com/global/opensource/files/32B650.zip \
	file://zecke-xgcc-cpp.patch;patch=1 \
	"

SRC_URI_append_samygo  = " file://SamyGO-Branding.patch;patch=1 "

do_unpack2() {
        tar --strip-components=2 -xvzf ${WORKDIR}/SELP.3.2.x-Chelsea.src.tgz SELP.3.2.x-Chelsea.src/Toolchain/gcc-4.2.0-4.0.9.tgz
        tar -xvzf gcc-4.2.0-4.0.9.tgz -C ${WORKDIR}/
	rm -f ${WORKDIR}/*.zip ${WORKDIR}/SELP* ${WORKDIR}/*.tgz ${WORKDIR}/*.gz || true
}

addtask unpack2 before do_patch after do_unpack

#Set the fortran bits
# 'fortran' or '', not 'f77' like gcc3 had
FORTRAN = ""
HAS_GFORTRAN = "no"
HAS_G2C = "no"

#Set the java bits
JAVA_arm = ""
JAVA = ""

LANGUAGES = "c,c++${FORTRAN}${JAVA}"
require gcc3-build.inc
ARCH_FLAGS_FOR_TARGET=-isystem${STAGING_INCDIR}

EXTRA_OECONF += " --disable-libssp --disable-bootstrap "

