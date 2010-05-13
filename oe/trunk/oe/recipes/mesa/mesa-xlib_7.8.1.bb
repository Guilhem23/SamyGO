include mesa-common.inc

PROTO_DEPS = "xf86driproto glproto"
LIB_DEPS = "virtual/libx11 libxext libxxf86vm libxdamage libxfixes"

DEPENDS = "${PROTO_DEPS}  ${LIB_DEPS}"

SRC_URI = "ftp://ftp.freedesktop.org/pub/mesa/${PV}/MesaLib-${PV}.tar.bz2;name=archive \
           ftp://ftp.freedesktop.org/pub/mesa/${PV}/MesaDemos-${PV}.tar.bz2;name=demos \
           file://fix-progs-makefile.patch;patch=1 \
	   file://fix-compile.patch;patch=1 \
          "

SRC_URI[archive.md5sum] = "25ec15f8e41fde6d206118cc786dbac4"
SRC_URI[archive.sha256sum] = "b0b46e5abfd75db44501e308125fa92bcf1c91d91e97a043a3b1764cfa0907fa"
SRC_URI[demos.md5sum] = "9ef47f911869657c6bf2f43ebce86b61"
SRC_URI[demos.sha256sum] = "e9f20b9345240064ac35ec914ebce63322a96d3f7c566963791d0daf7e7a93e5"

PR = "${INC_PR}.0"

#SamyGO: enable gallium software rasteriser
#EXTRA_OECONF += "--disable-gallium --with-driver=xlib"
EXTRA_OECONF += "--with-driver=xlib --enable-gallium-swrast"
