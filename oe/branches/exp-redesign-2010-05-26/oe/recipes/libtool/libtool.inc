DESCRIPTION = "Generic library support script \
This is GNU libtool, a generic library support script.  Libtool hides \
the complexity of generating special library types (such as shared \
libraries) behind a consistent interface."
HOMEPAGE = "http://www.gnu.org/software/libtool/libtool.html"
LICENSE = "GPL"
SECTION = "devel"

INC_PR = "r25"

FILESPATHPKG=. "libtool-${PV}:"

do_configure_prepend () {
	#SamyGO: fixed paths
	export ac_cv_path_SED="${STAGING_BINDIR_NATIVE}/sed"
	export ac_cv_path_GREP="${STAGING_BINDIR_NATIVE}/grep"
	export ac_cv_path_EGREP="${STAGING_BINDIR_NATIVE}/grep -E"
	export ac_cv_path_FGREP="${STAGING_BINDIR_NATIVE}/grep -F"
}
