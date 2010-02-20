LICENSE     = "LGPL"
DESCRIPTION = "Implementation of the Stringprep, Punycode and IDNA specifications defined by the IETF Internationalized Domain Names (IDN) working group."
SECTION = "libs"
PRIORITY    = "optional"
PR          = "r01"

DEPENDS = "virtual/libintl"

inherit pkgconfig autotools

SRC_URI = "ftp://alpha.gnu.org/gnu/libidn/${P}.tar.gz"

EXTRA_OECONF = " --disable-tld --disable-nls"

do_configure(){
	oe_runconf
}
do_compile(){
	oe_runmake
}

do_stage () {
	autotools_stage_all
}
