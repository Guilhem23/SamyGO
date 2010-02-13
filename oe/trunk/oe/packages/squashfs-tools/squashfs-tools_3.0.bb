require squashfs-tools.inc
PR = "${INC_PR}.1"
DEPENDS = "zlib"

SRC_URI += " file://Makefile.patch;patch=1 file://fix-return-ok.patch;patch=1"
