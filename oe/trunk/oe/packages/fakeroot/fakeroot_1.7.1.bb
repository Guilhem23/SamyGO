DESCRIPTION = "Gives a fake root environment"
SECTION = "base"
LICENSE = "GPL"
# fakeroot needs getopt which is provided by the util-linux package
RDEPENDS = "util-linux"
FILE_PR = "r2"

SRC_URI = "\
  http://stb.zsolttech.com/fakeroot_1.7.1.tar.gz \
  file://work-with-older-libtool.patch;patch=1 \
"

inherit autotools

do_stage() {
        install -d ${STAGING_INCDIR}/fakeroot
        install -m 644 *.h ${STAGING_INCDIR}/fakeroot
        autotools_stage_all
}
