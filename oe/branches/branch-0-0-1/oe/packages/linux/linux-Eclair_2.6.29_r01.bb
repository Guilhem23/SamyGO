DESCRIPTION = "Linux kernel for Samsung Mobile-Phones (Android)"
HOMEPAGE = "http://www.samsung.com/global/opensource/files"
LICENSE = "GPL"
# DEPENDS = "binutils-cross yes-native u-boot-mkimage-selp-native"

SRC_URI = "${SAMSUNG_OS_MIRROR}/GT-I5700_OpenSource_Update2.zip \
		${SAMSUNG_OS_MIRROR}/kernel_64m.tgz \
                file://crosscompiler_fix.patch;patch=1 \
"

S = "${WORKDIR}/I5700_Eclair_KernelOpenSource/kernel"

DEFAULT_PREFERENCE = "-1"
# INHIBIT_DEFAULT_DEPS = "1"

# KERNEL_VERSION_samygo = ${PV}${KERNEL_LOCALVERSION}

inherit kernel
DEPENDS = ""

FILES_dvb-includes = "/usr/include/linux/dvb"
FILES_scsi-includes = "/usr/include/linux/scsi"

COMPATIBLE_HOST = "arm.*-linux"
DEFAULT_PREFERENCE_spica = "10"

export OS = "Linux"
ARCH = "arm"

do_unpack2() {
        tar -xvzf ${WORKDIR}/I5700_Eclair_KernelOpenSource.tar.gz -C ${WORKDIR}/
	mv ${WORKDIR}/kernel_64m/drivers/xsr ${S}/../modules/
	cp -rf ${S}/../delta/* ${S}/../kernel
	cp ${S}/arch/arm/configs/spica_android_defconfig ${S}/.config
	sed -i '/CONFIG_INITRAMFS_SOURCE/d' ${S}/.config
	sed -i '63iCONFIG_INITRAMFS_SOURCE=""' ${S}/.config
	chmod a+w ${S}/.config
#	 cp -a /home/zsolt/kernel_64m/drivers/xsr /home/Software/OpenEmbedded/SamsungDTV-SELP/samygo-3.2/build/tmp/work/GT-I5700-SamyGO_OE.dev-linux-gnueabi/linux-Eclair-2.6.29-r01/I5700_Eclair_KernelOpenSource/modules/
        rm -f ${WORKDIR}/*gz || true
}

addtask unpack2 before do_patch after do_unpack

do_configure_prepend() {

	echo ${CROSS_COMPILE} > .cross_compile

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
}

# extra_mod_dirs = "dhd param vibrator multipdp dpram rfs xsr_stl xsr s3c_*"
extra_mod_dirs = "btgpio camera cmm dpram g2d g3d jpeg mfc multipdp param pp rotator vibetonz wlan"

MODULE_MAKE_FLAGS = '\
        KERNEL_PATH=${S}\
        KERNEL_SRC=${S}\
        KDIR=${S}\
        KERNELDIR=${S}\
        CC="${KERNEL_CC}" LD="${KERNEL_LD}"\
        AR="${KERNEL_AR}"\
	PRJROOT="${S}/../"\
        '

do_compile_append (){
#	unset CFLAGS CPPFLAGS CXXFLAGS LDFLAGS
	for i in ${extra_mod_dirs} ; do
		pushd ${S}/../modules/$i
        	oe_runmake ${MODULE_MAKE_FLAGS} -C ${S} M=`pwd` modules 
		popd
	done
}

do_install_append (){
#	unset CFLAGS CPPFLAGS CXXFLAGS LDFLAGS
#		MAKE_TARGETS=modules_install
	for i in ${extra_mod_dirs} ; do
		pushd ${S}/../modules/$i
		oe_runmake DEPMOD=echo INSTALL_MOD_PATH="${D}" ${MODULE_MAKE_FLAGS} -C ${S} M=`pwd` modules_install
		popd
	done
}

do_stage_append () {
        install ${S}/.cross_compile ${STAGING_KERNEL_DIR}/
}

do_rm_work () {
        oenote "we keep data for now!"
        :
}
