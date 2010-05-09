DESCRIPTION = "<description>"

PV = "0.0.1"
PR = "r0"

DEPENDS_append=" ${KERNEL_PACKAGE} "

INSTALL_PKGS = "\
	base-files base-passwd busybox kernel-modules sysvinit sysvinit-pidof libc6 libstdc++6 \
	netbase modutils-initscripts tinylogin initscripts tinylogin busybox-telnetd update-alternatives \
	e2fsprogs e2fsprogs-mke2fs util-linux-ng dosfstools tar gzip bzip2 module-init-tools-depmod \
	modutils-collateral gdb gdbserver samsung-apps samsung-original-apps-${FIRMWARE_NAME} \
"

#DEPENDS_append="xserver-xorg "
#INSTALL_PKGS_append = " x11vnc xserver-xorg-xvfb "

RRECOMMENDS = ""

IMAGE_FSTYPES = "tar.gz"
IMAGE_BASENAME = "externalboot-${FIRMWARE_NAME}"
IMAGE_LINGUAS = ""
IMAGE_INSTALL = "${INSTALL_PKGS} "

inherit image

