
require externalboot-common.inc

DEPENDS_append = " xserver-xorg "
INSTALL_PKGS_append = " x11vnc xserver-xorg-extension-glx xserver-xorg-extension-dri2 xserver-xorg-xvfb xterm \
       openbox openbox-theme-clearlooks "

IMAGE_FSTYPES = "tar.gz"
IMAGE_BASENAME = "externalboot-devel"
IMAGE_LINGUAS = ""
IMAGE_INSTALL = "${INSTALL_PKGS} "
