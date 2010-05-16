DESCRIPTION = "Export your X session on-the-fly via VNC"
HOMEPAGE = "http://www.karlrunge.com/x11vnc/"
AUTHOR = "Karl Runge"
SECTION = "x11/utils"
LICENSE = "GPL"
#SamyGO: do not depend on avahi
DEPENDS = "libxinerama openssl virtual/libx11 libxtst libxext jpeg zlib"
#DEPENDS = "libxinerama openssl virtual/libx11 libxtst libxext avahi jpeg zlib"

SRC_URI = "${SOURCEFORGE_MIRROR}/libvncserver/x11vnc-${PV}.tar.gz"

inherit autotools

SRC_URI[md5sum] = "61b4b21b31aca5c87c21d88efaf7dc50"
SRC_URI[sha256sum] = "8041e90267c37808749e423bcaa52bab64f4dd206c6b4508f57ac233ae7d6c26"
