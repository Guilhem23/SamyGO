FILESDIR = "${@os.path.dirname(bb.data.getVar('FILE',d,1))}/binutils-${PV}"
require binutils_${PV}.bb
require binutils-cross.inc

#SamyGO: without bfd
EXTRA_OECONF = "--with-sysroot=${STAGING_DIR_TARGET} \
	    --program-prefix=${TARGET_PREFIX} \
	    --disable-werror"

