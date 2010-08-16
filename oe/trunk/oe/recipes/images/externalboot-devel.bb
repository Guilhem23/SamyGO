
require externalboot-common.inc

DEPENDS_append = " xserver-xorg "
INSTALL_PKGS_append = " x11vnc xserver-xorg-extension-glx xserver-xorg-extension-dri2 xserver-xorg-xvfb xterm \
	openbox openbox-theme-clearlooks binutils gcc make patch m4 gdb python perl gdbserver automake autoconf \
	bison coreutils file gawk libtool pkgconfig sed expat fakeroot gettext ncurses openssl readline grep \
	strace ltrace libsdl-x11 \
"

IMAGE_FSTYPES = "tar.gz"
IMAGE_BASENAME = "externalboot-devel"
IMAGE_LINGUAS = ""
IMAGE_INSTALL = "${INSTALL_PKGS} "
