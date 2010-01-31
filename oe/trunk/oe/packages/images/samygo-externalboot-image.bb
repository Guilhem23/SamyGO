DESCRIPTION = "<description>"

PV = "0.0.1"
PR = "r0"
#PROVIDES += " ${MACHINE}-externalboot-image "

#DEPENDS = "virtual/libc base-files busybox sysvinit "
DEPENDS_sdp83 += "linux-chelsea-externalboot"

RDEPENDS = "\
	base-files base-passwd busybox busybox-mdev libc6 kernel-modules sysvinit sysvinit-pidof \
	netbase modutils-initscripts tinylogin initscripts tinylogin busybox-telnetd update-alternatives \
	e2fsprogs e2fsprogs-mke2fs util-linux dosfstools tar gzip bzip2 module-init-tools-depmod libstdc++6 \
	modutils-collateral samsung-apps \
"

RRECOMMENDS = ""

IMAGE_FSTYPES = "tar.gz"

export IMAGE_LINGUAS = " "
export IPKG_INSTALL = "${RDEPENDS}"

inherit image_ipk
