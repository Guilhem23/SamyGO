PR = "r6"
DESCRIPTION = "The GNU cc and gcc C compilers."
HOMEPAGE = "http://www.gnu.org/software/gcc/"
SECTION = "devel"
LICENSE = "GPL"

inherit autotools gettext

require gcc-package.inc

SRC_URI = "http://ftp.gnu.org/pub/gnu/gcc/gcc-4.1.1/gcc-4.1.1.tar.bz2 \
	file://gcc-4.1.1-st40r2-4.0.1.patch;patch=1 \
	file://gcc-4.1.1-st40r2-4.0.1-no-cbranchdi.patch;patch=1 \
	file://gcc-4.1.1-cross_search_paths-sh4.patch;patch=1 \
	file://gcc-${PV}-recursive_inline_fix.patch;patch=1 \
	file://gcc-4.1.1-linux-multilib-fix.patch;patch=1 \
	file://gcc-4.1.1-PR27781-fix.patch;patch=1 \
##	file://740-sh-pr24836.patch;patch=1 \
##	file://gcc41-configure.in.patch;patch=1 \
	file://ldflags.patch;patch=1 \
#	file://cse.patch;patch=1 \
#	file://zecke-xgcc-cpp.patch;patch=1 \
"

SRC_URI_append_fail-fast = " file://zecke-no-host-includes.patch;patch=1 "

#Set the fortran bits
FORTRAN = "" 
HAS_GFORTRAN = "no"
HAS_G2C = "no"

#Set the java bits
JAVA_arm = ""  
JAVA_sh4 = ""  

require gcc3-build.inc

EXTRA_OECONF += " --without-x "
EXTRA_OECONF_append_samygo = " --disable-doc "
