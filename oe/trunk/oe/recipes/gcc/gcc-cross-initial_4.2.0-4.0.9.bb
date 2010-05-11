require gcc-cross_${PV}.bb
require gcc-cross-initial.inc

EXTRA_OECONF = "--with-local-prefix=${STAGING_DIR_TARGET}${target_prefix} \
		--without-headers \
		--disable-shared \
		--disable-threads \
		--disable-multilib \
		--disable-__cxa_atexit \
		--enable-languages=c \
		--enable-target-optspace \
		--program-prefix=${TARGET_PREFIX} \
		--with-build-sysroot=${STAGING_DIR_TARGET} \
		${EXTRA_OECONF_INITIAL} \
		${@get_gcc_fpu_setting(bb, d)} \
		${@get_gcc_mips_plt_setting(bb, d)}"

# SamyGO: this cause broke compilation due lack std includes
#		--with-sysroot=${STAGING_DIR_TARGET} \

SRC_URI[md5sum] = "232c2397d511253a8d6dcf66fa8d9bc2"
SRC_URI[sha256sum] = "78cecf54357d014450355c35af21d2ad330c782a89f56fead6a65b238dbff0f6"
