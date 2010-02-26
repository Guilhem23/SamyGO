DESCRIPTION = "A tool for automatically generating Makefiles."
LICENSE = "GPL"
HOMEPAGE = "http://www.gnu.org/software/automake/"
SECTION = "devel"
RDEPENDS_automake += "\
    autoconf \
    perl \
    perl-module-bytes \
    perl-module-constant \
    perl-module-cwd \
    perl-module-data-dumper \
    perl-module-dynaloader \
    perl-module-errno \
    perl-module-exporter-heavy \
    perl-module-file-basename \
    perl-module-file-compare \
    perl-module-file-copy \
    perl-module-file-glob \
    perl-module-file-path \
    perl-module-file-spec-unix \
    perl-module-file-stat \
    perl-module-getopt-long \
    perl-module-io \
    perl-module-io-file \
    perl-module-posix \
    perl-module-strict \
    perl-module-text-parsewords \
    perl-module-vars "
SRC_URI = "${GNU_MIRROR}/automake/automake-${PV}.tar.bz2"
INC_PR = "r2"
AUTOMAKE_API = "${@".".join(bb.data.getVar("PV",d,1).split(".")[0:2])}"
FILESDIR = "${@os.path.dirname(bb.data.getVar('FILE',d,1))}/automake-${PV}"
S = "${WORKDIR}/automake-${PV}"

inherit autotools

export AUTOMAKE = "${@bb.which('automake', bb.data.getVar('PATH', d, 1))}"
FILES_${PN} += "${datadir}/automake* ${datadir}/aclocal*"

do_configure_prepend () {
	if ${@['true', 'false'][bb.data.inherits_class('native', d)]}
	then
		export ac_cv_path_PERL="${ac_cv_path_PERL=${bindir}/perl}"
	fi
}

do_install () {
	oe_runmake 'DESTDIR=${D}' install
	install -d ${D}${datadir}
	if [ ! -e ${D}${datadir}/aclocal ]; then
		ln -sf aclocal-${AUTOMAKE_API} ${D}${datadir}/aclocal
	fi
	if [ ! -e ${D}${datadir}/automake ]; then
		ln -sf automake-${AUTOMAKE_API} ${D}${datadir}/automake
	fi
}
