require squashfs-tools.inc
PR = "${INC_PR}.1"
DEPENDS = "zlib"

SRC_URI += " file://Makefile.patch;patch=1 file://fix-return-ok.patch;patch=1"

SRC_URI[md5sum] = "9fd05d0bfbb712f5fb95edafea5bc733"
SRC_URI[sha256sum] = "39dbda43cf118536deb746c7730b468702d514a19f4cfab73b710e32908ddf20"
