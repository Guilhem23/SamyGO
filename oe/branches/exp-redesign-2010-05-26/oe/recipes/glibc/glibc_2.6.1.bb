SRC_URI = "\
  ftp://ftp.gnu.org/pub/gnu/glibc/glibc-${PV}.tar.bz2;name=archive \
  ftp://ftp.gnu.org/pub/gnu/glibc/glibc-ports-${PV}.tar.bz2;name=ports \
  ftp://ftp.gnu.org/pub/gnu/glibc/glibc-libidn-${PV}.tar.bz2;name=libidn \
  file://arm-memcpy.patch;patch=1 \
  file://arm-longlong.patch;patch=1 \
  file://fhs-linux-paths.patch;patch=1 \
  file://dl-cache-libcmp.patch;patch=1 \
  file://nptl-crosscompile.patch;patch=1 \
  file://glibc-2.5-local-dynamic-resolvconf.patch;patch=1;pnum=0 \
  file://glibc-check_pf.patch;patch=1;pnum=0 \
  file://zecke-sane-readelf.patch;patch=1 \
  file://ldd-unbash.patch;patch=1 \
  file://generic-bits_select.h \
  file://generic-bits_types.h \
  file://generic-bits_typesizes.h \
  file://generic-bits_time.h \
  file://etc/ld.so.conf \
  file://generate-supported.mk \
  file://glibc-2.6.1-RTLD_SINGLE_THREAD_P-1.patch;patch=1 \
  file://glibc-2.6.1-use-short-for-fnstsw.patch;patch=1 \
  file://glibc-use-isystem-include-fixed.patch;patch=1 \
  file://glibc-arm-no-asm-page.patch;patch=1 \
  file://march-i686.patch;patch=1;pnum=0 \
"
