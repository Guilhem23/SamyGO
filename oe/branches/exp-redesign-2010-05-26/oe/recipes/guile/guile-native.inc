SECTION = "unknown"
LICENSE = "GPL"
DEPENDS = "gettext-native gmp-native"

inherit autotools native

S="${WORKDIR}/guile-${PV}"

#SamyGO:
EXTRA_OECONF += " export ac_cv_func_rl_get_keymap=no"

do_configure_append() {
        find ${S} -name Makefile | xargs sed -i s:'-Werror':'':g
}

#SamyGO:
do_compile_prepend_darwin() {
        for i in $(find ${S} -name "Makefile") ; do
                sed -i -e s:-Werror::g $i
        done

        echo "#undef LTDL_DLOPEN_DEPLIBS" >> config.h
        echo "#define LTDL_SHLIB_EXT \".so\"" >> config.h
        echo "#undef HAVE_STAT64" >> config.h

        export CFLAGS="-Dmacosx"
}

