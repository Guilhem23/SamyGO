DESCRIPTION = "<description>"

PV = "0.0.1"
PR = "r0"

inherit image_ipk

COMPATIBLE_MACHINE = "sdp83"

DEPENDS_append_sdp83="linux-chelsea-externalboot "

DEPENDS_append="xserver-xorg "

RDEPENDS = "\
	base-files base-passwd busybox busybox-mdev libc6 kernel-modules sysvinit sysvinit-pidof \
	netbase modutils-initscripts tinylogin initscripts tinylogin busybox-telnetd update-alternatives \
	e2fsprogs e2fsprogs-mke2fs util-linux dosfstools tar gzip bzip2 module-init-tools-depmod libstdc++6 \
	modutils-collateral samsung-apps samsung-original-apps x11vnc xserver-xorg-xvfb \
"

RRECOMMENDS = ""

IMAGE_FSTYPES = "tar.gz"

export IMAGE_LINGUAS = " "
export IPKG_INSTALL = "${RDEPENDS}"

