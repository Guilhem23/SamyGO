S = "${WORKDIR}/ipkg-${PV}"

#require ipkg_${PV}.bb
require ipkg-native.inc
PR = "r2"

inherit autotools pkgconfig native

# SRC_URI = "http://www.handhelds.org/pub/packages/ipkg/ipkg-${PV}.tar.gz \
SRC_URI = "http://stb.zsolttech.com/global/opensource/files/ipkg-${PV}.tar.gz \
	file://fix_compatibility.patch;patch=1 \
	file://fix_make_directory.patch;patch=1"
