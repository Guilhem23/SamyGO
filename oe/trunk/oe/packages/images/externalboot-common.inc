DESCRIPTION = "<description>"

PV = "0.0.1"
PR = "r0"

inherit image_ipk

DEPENDS_append=" ${KERNEL_PACKAGE} "

RDEPENDS = "\
	base-files base-passwd busybox busybox-mdev libc6 kernel-modules sysvinit sysvinit-pidof \
	netbase modutils-initscripts tinylogin initscripts tinylogin busybox-telnetd update-alternatives \
	e2fsprogs e2fsprogs-mke2fs util-linux dosfstools tar gzip bzip2 module-init-tools-depmod libstdc++6 \
	modutils-collateral gdb samsung-apps samsung-original-apps-${FIRMWARE_NAME} \
"

#DEPENDS_append="xserver-xorg "
#RDEPENDS_append = " x11vnc xserver-xorg-xvfb"

RRECOMMENDS = ""

IMAGE_FSTYPES = "tar.gz"

export IMAGE_BASENAME = "externalboot-${FIRMWARE_NAME}"
export IMAGE_LINGUAS = " "
export IPKG_INSTALL = "${RDEPENDS}"

