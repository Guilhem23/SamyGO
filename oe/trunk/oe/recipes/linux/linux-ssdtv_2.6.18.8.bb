DESCRIPTION = "Linux Kernel for Samsung TV's"
LICENSE = "GPLv2"
DEPENDS = "binutils-cross u-boot-mkimage-native"

SRC_URI = "${KERNELORG_MIRROR}/pub/linux/kernel/v2.6/linux-${PV}.tar.bz2 \
	file://arm-mm-2.6.19.patch;patch=1 \
	file://rt-2.6.18.8-stripped.patch;patch=1 \
	file://fix_nonlinux_compile.patch;patch=1 \
	file://ignore-mod-magic-version.patch;patch=1 \
	file://selp-arm.patch;patch=1 \
	file://selp-core.patch;patch=1 \
	file://selp-disk.patch;patch=1 \
	file://selp-div64.patch;patch=1 \
	file://selp-fs.patch;patch=1 \
	file://selp-irq.patch;patch=1 \
	file://selp-mm.patch;patch=1 \
	file://selp-net.patch;patch=1 \
	file://selp-ntfs.patch;patch=1 \
	file://selp-scsi.patch;patch=1 \
	file://selp-usb.patch;patch=1 \
	file://selp-vfp.patch;patch=1 \
	file://debian.patch;patch=1 \
	file://gadget.patch;patch=1 \
	file://pegasus.patch;patch=1 \
	file://ssdtv-platform.patch;patch=1 \
	file://defconfig_ssdtv \
"

# comment out below line to enable pci hack for b6000
#SRC_URI += "file://b6000-pci-hack.patch;patch=1 "

SRC_URI[md5sum] = "dce47badc1faf34b355a10b97ae5d391"
SRC_URI[sha256sum] = "945b3014f8048cd87fdff90014afa4ff241f134bceafbfdbd42dba1be8df2ba8"

S = "${WORKDIR}/linux-2.6.18.8"

KERNEL_VERSION = "2.6.18.8"
inherit kernel siteinfo

COMPATIBLE_HOST = "arm.*-linux"
COMPATIBLE_MACHINE = "ssdtv"

export ARCH = "arm"
export OS = "Linux"

UBOOT_ENTRYPOINT = "0x60008000"

do_install_append() {
	oe_runmake headers_install INSTALL_HDR_PATH=${D}${exec_prefix}/src/linux-${KERNEL_VERSION} ARCH=$ARCH
}

PACKAGES =+ "kernel-headers"
FILES_kernel-headers = "${exec_prefix}/src/linux*"

do_configure_prepend () {
	cp ${WORKDIR}/defconfig_ssdtv .config
	make include/linux/version.h
}

KERNEL_IMAGETYPE = "uImage"

