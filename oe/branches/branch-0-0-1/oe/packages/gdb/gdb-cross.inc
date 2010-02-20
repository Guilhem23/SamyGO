require gdb-common.inc

DEPENDS = "ncurses-native"

EXTRA_OEMAKE = "'SUBDIRS=intl mmalloc libiberty opcodes bfd sim gdb etc utils' LDFLAGS='${BUILD_LDFLAGS}'"

EXTRA_OECONF = "--with-curses --with-readline"
# EXTRA_OECONF_append_arm = " --enable-sim=yes --enable-sim-arm --enable-sim-stdio"
# EXTRA_OECONF_append_arm = " --target=arm-linux-eabi --program-transform-name='s/-eabi/-gnueabi/'"
EXTRA_OECONF_append_arm = " --target=arm-linux-eabi --program-prefix=${TARGET_PREFIX}"

do_configure () {
# override this function to avoid the autoconf/automake/aclocal/autoheader
# calls for now
	(cd ${S} && gnu-configize) || die "failure in running gnu-configize"

        # Fix for issues when system's texinfo version >= 4.10
        # (See https://bugzilla.redhat.com/show_bug.cgi?id=345621)
        sed -i -e "s@egrep 'texinfo.*'@egrep 'texinfo[^0-9]*([1-3][0-9]|4\.[4-9]|4.[1-9][0-9]+|[5-9])'@" '${S}/configure'


	oe_runconf
}

do_stage () {
	oe_runmake install
}
