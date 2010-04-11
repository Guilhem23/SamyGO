include mesa-common.inc

PROTO_DEPS = "glproto"
LIB_DEPS = "virtual/libx11 libxext libxdamage libxfixes"

DEPENDS = "${PROTO_DEPS}  ${LIB_DEPS}"

SRC_URI = "ftp://ftp.freedesktop.org/pub/mesa/${PV}/MesaLib-${PV}.tar.bz2 \
           ftp://ftp.freedesktop.org/pub/mesa/${PV}/MesaDemos-${PV}.tar.bz2 \
	   file://fix-compile.patch;patch=1 \
          "

PE = "1"
PR = "${INC_PR}.0"

#EXTRA_OECONF += "--disable-gallium --with-driver=xlib"
EXTRA_OECONF += "--with-driver=xlib --enable-gallium-swrast"
