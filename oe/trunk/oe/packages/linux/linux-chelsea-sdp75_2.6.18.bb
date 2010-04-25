DESCRIPTION = "Linux kernel for Samsung TV's"
HOMEPAGE = "http://www.samsung.com/global/opensource/files"
LICENSE = "GPL"
DEPENDS = "binutils-cross yes-native u-boot-mkimage-selp-native"

SRC_URI = "http://www.samsung.com/global/opensource/files/LN52A750.zip \
	file://selp-a1-fix_nonlinux_compile.patch;patch=1 \
	file://selp-gadget.patch;patch=1 \
	file://${KERNEL_DEFCONFIG} \
"

S = "${WORKDIR}/linux"

KERNEL_VERSION_samygo = ${PV}
KERNEL_LOCALVERSION = "_SELP-ARM_V403_V6K"
inherit kernel

FILES_dvb-includes = "/usr/include/linux/dvb"
FILES_scsi-includes = "/usr/include/linux/scsi"

COMPATIBLE_HOST = "arm.*-linux"
COMPATIBLE_MACHINE = "sdp75"

DEFAULT_PREFERENCE = "-1"

KERNEL_DEFCONFIG = "defconfig_sdp75"

export OS = "Linux"
ARCH = "arm"

do_unpack2() {
        tar -xvf ${WORKDIR}/linux_A1.tar -C ${WORKDIR}/
	rm -f ${WORKDIR}/*.zip ${WORKDIR}/SELP* ${WORKDIR}/*.tgz ${WORKDIR}/*.gz ${WORKDIR}/*.tar || true
}

addtask unpack2 before do_patch after do_unpack

do_configure_prepend() {
	echo ${CROSS_COMPILE} > .mvl_cross_compile
	echo ${TARGET_ARCH} > .mvl_target_cpu
	cp ${WORKDIR}/${KERNEL_DEFCONFIG} .config
	make include/linux/version.h
}

do_stage_prepend () {
	rm -f ${S}/include/asm-${TARGET_ARCH}/arch-ssdtv || true
	rm -f ${S}/include/asm-${TARGET_ARCH}/arch || true
	cp -avf ${WORKDIR}/linux/include/asm-${TARGET_ARCH}/arch-ssdtv ${S}/include/asm-${TARGET_ARCH}/ || true
	cp -avf ${WORKDIR}/linux/include/asm-${TARGET_ARCH}/arch-ssdtv ${S}/include/asm/arch || true
}

do_stage_append () {
	install ${S}/.mvl_cross_compile ${STAGING_KERNEL_DIR}/
	install ${S}/.mvl_target_cpu ${STAGING_KERNEL_DIR}/
}

KERNEL_IMAGETYPE_LIST = "Image uImage"

