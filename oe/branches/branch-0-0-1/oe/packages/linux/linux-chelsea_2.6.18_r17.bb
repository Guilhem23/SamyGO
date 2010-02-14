DESCRIPTION = "Linux kernel for Samsung TV's"
HOMEPAGE = "http://www.samsung.com/global/opensource/files"
LICENSE = "GPL"
DEPENDS = "binutils-cross yes-native u-boot-mkimage-selp-native"

# LN46A750.zip & LN52A750.zip same md5sum
SRC_URI = "http://www.samsung.com/global/opensource/files/LN52A750.zip \
		file://${MACHINE}-dotconfig \
		file://selp-fix-MAX_PATH.patch;patch=1 \
"
SRC_URI_append_samygo += "file://selp-gadget.patch;patch=1" 

S = "${WORKDIR}/linux"

KERNEL_VERSION_samygo = ${PV}
KERNEL_LOCALVERSION = "_SELP-ARM_V403_V6K" 
inherit kernel

FILES_dvb-includes = "/usr/include/linux/dvb"
FILES_scsi-includes = "/usr/include/linux/scsi"

COMPATIBLE_HOST = "arm.*-linux"
COMPATIBLE_MACHINE = "(T-SPHAUSC|T-RBYAUSC)"

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

	oe_machinstall -m 0644 ${WORKDIR}/${MACHINE}-dotconfig ${S}/.config

	gcc_version=`${KERNEL_CC} -dumpversion`
                if [ "${gcc_version}" == "4.0.1" ] || [ "${gcc_version}" == "4.0.2" ]; then
                        die "tosa kernel wont work with gcc 4.0.x"
                fi
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

