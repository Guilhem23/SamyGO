require gcc-cross_${PV}.bb
require gcc-cross-initial.inc

EXTRA_OECONF = "--with-local-prefix=${STAGING_DIR_TARGET}${target_prefix} \
		--without-headers \
		--disable-shared \
		--disable-threads \
		--disable-multilib \
		--disable-__cxa_atexit \
		--disable-libmudflap \
		--disable-libssp \
		--disable-libgomp \
		--enable-languages=c \
		--enable-target-optspace \
		--program-prefix=${TARGET_PREFIX} \
		--with-build-sysroot=${STAGING_DIR_TARGET} \
		${@get_gcc_fpu_setting(bb, d)} \
		${@get_gcc_mips_plt_setting(bb, d)}"

# SamyGO: this cause broke compilation due lack std includes
#		--with-sysroot=${STAGING_DIR_TARGET} \
