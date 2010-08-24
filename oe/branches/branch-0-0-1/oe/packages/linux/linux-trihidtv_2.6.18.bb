DESCRIPTION = "Linux kernel for Samsung TV's"
HOMEPAGE = "http://www.samsung.com/global/opensource/files"
LICENSE = "GPL"
# DEPENDS = "binutils-cross yes-native u-boot-mkimage-selp-native"

SRC_URI = "${SAMSUNG_OS_MIRROR}/LN40C550.zip \
                file://${MACHINE}-dotconfig \
"
SRC_URI_append_samygo += "file://selp-gadget.patch;patch=1 \
#                file://selp-ralink-devlist.patch;patch=1;pnum=0 \
#                file://selp-ralink-devlist_2.2.0.0.patch;patch=1;pnum=0 \
#                file://selp_experimantal_d-link_110.patch;patch=1 \
"
S = "${WORKDIR}/mv_selp_kernel_1C_3B_RFS131_FSR120/kernel-2.6.18_pro500-mips24ke_nfp_be"

DEFAULT_PREFERENCE = "-1"
# INHIBIT_DEFAULT_DEPS = "1"

KERNEL_VERSION_samygo = ${PV}${KERNEL_LOCALVERSION}

inherit kernel
DEPENDS = ""

FILES_dvb-includes = "/usr/include/linux/dvb"
FILES_scsi-includes = "/usr/include/linux/scsi"

COMPATIBLE_HOST = "mips.*-linux"
# COMPATIBLE_MACHINE = "T-CH.*C"
# COMPATIBLE_MACHINE_CLASS = "leonid"
DEFAULT_PREFERENCE_trihidtv = "10"

export OS = "Linux"
ARCH = "mips"

do_unpack2() {
        tar -xvzf ${WORKDIR}/linux.tgz -C ${WORKDIR}/
# rtnet3572sta.tgz rtutil3572sta.tgz 
        rm -f ${WORKDIR}/*.tar ${WORKDIR}/SELP* ${WORKDIR}/*.tgz ${WORKDIR}/*.gz || true
        # Branding
        perl -pi -e "s/0.994/0.994 \[SamyGO\]/g" ${WORKDIR}/linux/linux-r011/drivers/rt73/Module/rtmp_main.c
        perl -pi -e "s/1.002\(1.4.0.0\)/1.002\(1.4.0.0\) \[SamyGO\]/g" ${WORKDIR}/linux/linux-r011/drivers/rt2870/os/linux/2870_main_dev.c
}

addtask unpack2 before do_patch after do_unpack

do_configure_prepend() {
	echo ${CROSS_COMPILE} > .mvl_cross_compile
	echo ${TARGET_ARCH} > .mvl_target_cpu

	oe_runmake include/linux/version.h
	
#	oe_machinstall -m 0644 ${WORKDIR}/${MACHINE}-dotconfig ${S}/.config
#	oe_runmake oldconfig
	if [ "${MACHINE}" = "T-DUMMY" ] ; then
        oe_machinstall -m 0644 ${WORKDIR}/${MACHINE}-dotconfig ${S}/.config
	rm -fv include/asm/arch
	oe_runmake oldconfig
        oe_runmake prepare
	else	

        perl -pi -e "s/# (CONFIG_BLK_DEV_LOOP) .*/\1=m/" ${S}/.config
        perl -pi -e "s/# (CONFIG_SMB_FS) .*/\1=m/" ${S}/.config
# CONFIG_SMB_NLS_DEFAULT is not set
perl -pi -e "s/# (CONFIG_CIFS) .*/\1=m\nCONFIG_CIFS_EXPERIMENTAL=y/" ${S}/.config
perl -pi -e "s/# (CONFIG_CODA_FS) .*/\1=m/" ${S}/.config
# we use fuse api >= 7.8 so build it outside of kernel
# perl -pi -e "s/# (CONFIG_FUSE_FS) .*/\1=m/" ${S}/.config
perl -pi -e "s/# (CONFIG_ISO9660_FS) .*/\1=m\nCONFIG_JOLIET=y/" ${S}/.config
perl -pi -e "s/# (CONFIG_UDF_FS) .*/\1=m\nCONFIG_UDF_NLS=y/" ${S}/.config
perl -pi -e "s/# (CONFIG_HFS_FS) .*/\1=m/" ${S}/.config
perl -pi -e "s/# (CONFIG_HFSPLUS_FS) .*/\1=m/" ${S}/.config

perl -pi -e "s/# (CONFIG_USB_GADGET) .*/\1=m/" ${S}/.config

perl -pi -e "s/# (CONFIG_BT) .*/\1=m/" ${S}/.config

# evdev
perl -pi -e "s/# (CONFIG_INPUT_EVDEV) .*/\1=m/" ${S}/.config

# usb hid
perl -pi -e "s/# (CONFIG_USB_HID) .*/\1=m\nCONFIG_USB_HIDINPUT=y/" ${S}/.config

# usb cdrom & pktdrv
perl -pi -e "s/# (CONFIG_CDROM_PKTCDVD) .*/\1=m\nCONFIG_CDROM_PKTCDVD_BUFFERS=8/" ${S}/.config
perl -pi -e "s/# (CONFIG_BLK_DEV_SR) .*/\1=m/" ${S}/.config
perl -pi -e "s/# (CONFIG_CHR_DEV_SG) .*/\1=m/" ${S}/.config

echo 'CONFIG_BT_L2CAP=m
CONFIG_BT_SCO=m
CONFIG_BT_RFCOMM=m
# CONFIG_BT_RFCOMM_TTY is not set
CONFIG_BT_BNEP=m
CONFIG_BT_BNEP_MC_FILTER=y
CONFIG_BT_BNEP_PROTO_FILTER=y
CONFIG_BT_HIDP=m
#
# Bluetooth device drivers
#
CONFIG_BT_HCIUSB=m
CONFIG_BT_HCIUSB_SCO=y
# CONFIG_BT_HCIUART is not set
# CONFIG_BT_HCIBCM203X is not set
# CONFIG_BT_HCIBPA10X is not set
# CONFIG_BT_HCIBFUSB is not set
# CONFIG_BT_HCIVHCI is not set
' >> ${S}/.config 

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
' >> ${S}/.config
            fi            
}

do_stage_append () {
	install ${S}/.mvl_cross_compile ${STAGING_KERNEL_DIR}/ 
	install ${S}/.mvl_target_cpu ${STAGING_KERNEL_DIR}/
}

do_rm_work () {
        oenote "we keep data for now!"
        :
}
