DESCRIPTION = "<description>"

PV = "0.0.1"
PR = "r0"
PROVIDES += " ${MACHINE}-nfsboot-image "

DEPENDS = "task-samsung-toolchain base-files busybox virtual/kernel "
RDEPENDS = " base-files busybox busybox-mdev libc6 kernel-module-usbcore kernel-module-8139too kernel-module-ehci-hcd kernel-module-fsr-stl \
kernel-module-fsr kernel-module-ohci-hcd kernel-module-rfs kernel-module-rt2870sta kernel-module-rt73 \
"
RRECOMMENDS = ""

IMAGE_FSTYPES = "tar.gz"

export IMAGE_LINGUAS = " "
export IPKG_INSTALL = "${RDEPENDS}"

inherit image_ipk
