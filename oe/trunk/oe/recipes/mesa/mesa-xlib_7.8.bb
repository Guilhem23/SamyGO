include mesa-common.inc

PROTO_DEPS = "xf86driproto glproto"
LIB_DEPS = "virtual/libx11 libxext libxxf86vm libxdamage libxfixes"

DEPENDS = "${PROTO_DEPS}  ${LIB_DEPS}"

SRC_URI = "ftp://ftp.freedesktop.org/pub/mesa/${PV}/MesaLib-${PV}.tar.bz2;name=archive \
           ftp://ftp.freedesktop.org/pub/mesa/${PV}/MesaDemos-${PV}.tar.bz2;name=demos \
           file://fix-progs-makefile.patch;patch=1 \
          "

#SRC_URI[archive.md5sum] = "85cb891eecb89aae4fdd3499cccd934b"
#SRC_URI[archive.sha256sum] = "8c85db5844303b806b18fc6bd40a9dccb02d90b54878a94f910674673ba0aa35"
SRC_URI[demos.md5sum] = "85cb891eecb89aae4fdd3499cccd934b"
SRC_URI[demos.sha256sum] = "8c85db5844303b806b18fc6bd40a9dccb02d90b54878a94f910674673ba0aa35"

PR = "${INC_PR}.0"

#SamyGO: enable gallium software rasteriser
#EXTRA_OECONF += "--disable-gallium --with-driver=xlib"
EXTRA_OECONF += "--with-driver=xlib --enable-gallium-swrast"
