DESCRIPTION = "<description>"

PV = "0.0.1"
PR = "r0"
PROVIDES += " ${MACHINE}-nfsboot-image "

DEPENDS = "virtual/libc base-files busybox "
DEPENDS_sdp83 += "linux-chelsea-nfsboot"

RDEPENDS = "\
	base-files busybox busybox-mdev libc6 kernel-modules \
"

RRECOMMENDS = ""

IMAGE_FSTYPES = "tar.gz"

export IMAGE_LINGUAS = " "
export IPKG_INSTALL = "${RDEPENDS}"

inherit image_ipk
