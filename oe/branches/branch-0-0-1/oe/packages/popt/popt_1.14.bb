require popt.inc

PR = "r01"

DEPENDS = "gettext virtual/libintl libiconv"
DEPENDS_samygo = "virtual/libintl"


SRC_URI = "\
  http://rpm5.org/files/popt/${P}.tar.gz \
"

inherit autotools autotools_stage pkgconfig 
EXTRA_OECONF += " --disable-nls --with-included-gettext "

do_configure() {
	oe_runconf
}

do_stage () {
        autotools_stage_includes
        oe_libinstall -so -C .libs libpopt ${STAGING_LIBDIR}/
}
