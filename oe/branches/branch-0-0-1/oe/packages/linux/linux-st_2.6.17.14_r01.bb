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
KERNEL_LOCALVERSION = "_stm22_0039-Samsung_7103" 
inherit kernel

FILES_dvb-includes = "/usr/include/linux/dvb"
FILES_scsi-includes = "/usr/include/linux/scsi"

COMPATIBLE_HOST = "sh.*-linux"
COMPATIBLE_MACHINE = "T-RBYDEUC"
COMPATIBLE_MACHINE = "T-RB.*C"

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
	oe_machinstall -m 0644 samsung_7103_kernel-2.2.cfg .config
	
	perl -pi -e "s/# (CONFIG_USB_GADGET) .*/\1=m/" ${S}/.config
        echo '# USB Gadget Support
#
# CONFIG_USB_GADGET_DEBUG_FILES is not set
CONFIG_USB_GADGET_SELECTED=y
# CONFIG_USB_GADGET_NET2280 is not set
# CONFIG_USB_GADGET_PXA2XX is not set
# CONFIG_USB_GADGET_GOKU is not set
# CONFIG_USB_GADGET_LH7A40X is not set
# CONFIG_USB_GADGET_OMAP is not set
# CONFIG_USB_GADGET_AT91 is not set
CONFIG_USB_GADGET_DUMMY_HCD=y
CONFIG_USB_DUMMY_HCD=m
CONFIG_USB_GADGET_DUALSPEED=y
# CONFIG_USB_ZERO is not set
# CONFIG_USB_ETH is not set
# CONFIG_USB_GADGETFS is not set
CONFIG_USB_FILE_STORAGE=m
CONFIG_USB_FILE_STORAGE_TEST=y
# CONFIG_USB_G_SERIAL is not set
CONFIG_CIFS=m
' >> ${S}/.config

	gcc_version=`${KERNEL_CC} -dumpversion`
                if [ "${gcc_version}" == "4.0.1" ] || [ "${gcc_version}" == "4.0.2" ]; then
                        die "tosa kernel wont work with gcc 4.0.x"
                fi
}

