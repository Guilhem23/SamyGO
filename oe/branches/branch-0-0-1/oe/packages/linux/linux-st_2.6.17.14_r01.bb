DESCRIPTION = "Linux kernel for Samsung TV's"
HOMEPAGE = "http://www.samsung.com/global/opensource/files"
LICENSE = "GPL"
DEPENDS = "binutils-cross yes-native"

SRC_URI = "http://www.samsung.com/global/opensource/files/LE46A956.zip \
		file://${MACHINE}-dotconfig \
		file://selp-fix-MAX_PATH.patch;patch=1 \
"
SRC_URI_append_samygo += "file://selp-gadget.patch;patch=1" 

S = "${WORKDIR}/linux_st"

KERNEL_CCSUFFIX = "-4.1.1"

KERNEL_VERSION_samygo = ${PV}
KERNEL_LOCALVERSION = "-Samsung_7103" 
inherit kernel

FILES_dvb-includes = "/usr/include/linux/dvb"
FILES_scsi-includes = "/usr/include/linux/scsi"

COMPATIBLE_HOST = "sh.*-linux"
COMPATIBLE_MACHINE = "T-RBYDEUC"

export OS = "Linux"
ARCH = "sh"
KERNEL_OUTPUT = "arch/${ARCH}/boot/zImage"
KERNEL_OBJECT_SUFFIX = '.ko'

do_unpack2() {
        tar -xvzf ${WORKDIR}/linux_st.tar.gz -C ${WORKDIR}/
	rm -f ${WORKDIR}/*.zip ${WORKDIR}/SELP* ${WORKDIR}/*.tgz ${WORKDIR}/*.gz ${WORKDIR}/Re* ${WORKDIR}/*.bz2 || true
}

addtask unpack2 before do_patch after do_unpack

do_configure_prepend() {
	rm include/asm-sh/mach/stb7100ref && tar xvzf arch/sh/boards/st/stb7100ref.tar.gz -C include/asm-sh/
	cp samsung_7103_kernel-2.2.cfg .config.old
	oe_runmake oldconfig

	gcc_version=`${KERNEL_CC} -dumpversion`
                if [ "${gcc_version}" == "4.0.1" ] || [ "${gcc_version}" == "4.0.2" ]; then
                        die "tosa kernel wont work with gcc 4.0.x"
                fi
}

