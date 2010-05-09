FILESPATHPKG =. "binutils-${PV}:"
require binutils_${PV}.bb
require binutils-cross.inc

EXTRA_OECONF = "--with-sysroot=${STAGING_DIR_TARGET} \
	    --program-prefix=${TARGET_PREFIX} \
	    --disable-werror"

