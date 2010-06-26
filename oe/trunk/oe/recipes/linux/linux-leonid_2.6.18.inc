DESCRIPTION = "Linux kernel for Samsung TV's"
HOMEPAGE = "http://www.samsung.com/global/opensource/files"
LICENSE = "GPL"
DEPENDS = "binutils-cross u-boot-mkimage-native"

# SamyGO: download manualy LE32B650.zip package sources for TV LE32B650
# from http://opensource.samsung.com site, it is called there le32b650
# and copy it to OE source directory.

SRC_URI = "http://opensource.samsung.com/tv_n_video/le32b650/LE32B650.zip \
	file://selp-fix_nonlinux_compile.patch;patch=1 \
	file://selp-gadget.patch;patch=1 \
	file://pegasus.patch;patch=1 \
	file://ignore-mod-magic-version.patch;patch=1 \
	file://${KERNEL_DEFCONFIG} \
"

SRC_URI[md5sum] = "42d564a271520a114710c2964efdf716"
SRC_URI[sha256sum] = "98d0991acb045d537c6c6b05cb990d70eb8390b83a1994472b958c72ff7eb02f"

S = "${WORKDIR}/linux/linux-r011"

KERNEL_VERSION = "2.6.18"
inherit kernel

FILES_dvb-includes = "/usr/include/linux/dvb"
FILES_scsi-includes = "/usr/include/linux/scsi"

COMPATIBLE_HOST = "arm.*-linux"
COMPATIBLE_MACHINE = "ssdtv"

DEFAULT_PREFERENCE = "-1"

export ARCH = "arm"
export OS = "Linux"

do_unpack2() {
	tar -xvzf ${WORKDIR}/linux_b678eu.tgz -C ${WORKDIR}/
	rm -f ${WORKDIR}/*.tgz ${WORKDIR}/*.gz || true
	echo "#define UTS_RELEASE \"2.6.18\"" > ${WORKDIR}/linux/linux-r011/include/linux/utsrelease.h
}

addtask unpack2 before do_patch after do_unpack

do_configure_prepend () {
	echo ${CROSS_COMPILE} > .mvl_cross_compile
	echo ${TARGET_ARCH} > .mvl_target_cpu
	echo > localversion
	echo > localversion-mvl
	cp ${WORKDIR}/${KERNEL_DEFCONFIG} .config
	make include/linux/version.h
}

KERNEL_IMAGETYPE_LIST = "Image uImage"

