require gdb.inc
LICENSE = "GPLv3"

SRC_URI += "\
            file://gdb-6.8-fix-compile-karmic.patch;patch=1"

# Work-around problems while creating libbfd.a
EXTRA_OECONF += "--enable-static"

do_configure_prepend_sh4(){
#
# * Arris *
# for sh4
# have to check libiberty.h no define for PEX_STDERR_ (binutils??)
# gdb-7.0.1/gdb/configure.host missing sh*-*-linux*)           gdb_host=linux ;;
# so host_makefile_frag points to
# host_makefile_frag='.../gdb-7.0.1/gdb/config/sh/.mh' 
# and not to (f...ing missing file)
# host_makefile_frag='../gdb-7.0.1/gdb/config/sh/linux.mh'
#
# but i guess you need the package gdbserver and gdb-cross and not gdb
# look at: http://www.linux.com/archive/feature/121735
# if the build of gdbserver fail with
# error: expected '=', ',', ';', 'asm' or '__attribute__' before 'Elf_External_Versym'
# remove build/tmp/staging/sh4-linux/include/ansidecl.h
#
oefatal "this isn't the package you want, look at: http://www.linux.com/archive/feature/121735"
}  
