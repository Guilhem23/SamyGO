DESCRIPTION = "Tool to work with series of patches."
HOMEPAGE = "http://savannah.nongnu.org/projects/quilt/"
SECTION = "devel"
LICENSE = "GPL"

#SamyGO: removed no_rm_symlinks.patch
SRC_URI = "http://download.savannah.gnu.org/releases/quilt/quilt-${PV}.tar.gz \
           file://install.patch \
	   "

S = "${WORKDIR}/quilt-${PV}"
