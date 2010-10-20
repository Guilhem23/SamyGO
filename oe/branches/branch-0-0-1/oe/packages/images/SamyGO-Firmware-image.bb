# Image configuration for the SamyGO Flasable kernel 
#
#
# Copyright Ser Lev Arris <arris@ZsoltTech.Com> (c) 2009-2010
# License: GPL (see http://www.gnu.org/licenses/gpl.txt for a copy of the license)
#
# Filename: ${DISTRO_NAME}-Flash-Image.bb
# Date: 19-Oct-2010 
# $Id: SamyGO-Flash-Image_v0.01.1.bb 1049 2010-10-19 18:05:13Z arris $

DESCRIPTION = "<description>"
MAINTAINER = "Ser Lev Arris <arris@ZsoltTech.Com>"
HOMEPAGE = "http://stb.ZsoltTech.Com"

PV = "v0.01.1"
PR = "r01"
export IMAGE_BASENAME = "${PN}-${PV}-${PR}-for-${TARGET_ARCH}"

FEED_URIS = ""

FEED_DEPLOYDIR_BASE_URI = ""

IMAGE_INSTALL = "${INSTALL_PACKAGES} "
IMAGE_LINGUAS = ""

IMAGE_RDEPENDS = ""

INSTALL_PACKAGES = " \
kernel-image \
kernel-modules \
wlan-rt25usb wlan-rt3070usb ulan-dm9601 \
rt2570-k2wrlz \
"

INSTALL_PACKAGES_append_arm += " samsung-firmware-update-tools"
INSTALL_PACKAGES_append_mips += " "
INSTALL_PACKAGES_append_sh4 += " "

IMAGE_LOGIN_MANAGER = ""

IMAGE_DEV_MANAGER = ""
IMAGE_INIT_MANAGER = ""
IMAGE_INITSCRIPTS = ""

IMAGE_DEVICE_TABLES = ""

IMAGE_FSTYPES = "zip"
# IMAGE_ROOTFS_SIZE_ext2="10240"
EXTRA_IMAGECMD_jffs2=""
# FIRMWARE_CLASS or MACHINE ?
IMAGE_CMD_zip = " \
        cd ${IMAGE_ROOTFS} && zip ${EXTRA_IMAGECMD} \
        ${DEPLOY_DIR_IMAGE}/${IMAGE_NAME}.zip ${MACHINE} boot README"
EXTRA_IMAGECMD_zip = "-r"

ROOTFS_POSTPROCESS_COMMAND = "rootfs_postprocess"

UBOOT_SYMLINK ?= "u-boot-${MACHINE}.bin"

python __anonymous () {
    import bb, re 
    fw_ver = re.sub('\+Samsung', '', bb.data.getVar('PREFERRED_VERSION_samsung-firmware', d, 1))
    bb.data.setVar('FW_VER', fw_ver, d)
    fw_part = fw_ver.split(".")
    bb.data.setVar('FW_MAJ', fw_part[0], d)
    bb.data.setVar('FW_MIN', fw_part[1], d)
    bb.data.setVar('FW_MAJO', "00" + "%s" % fw_part[0], d)
}

FW_DATE = "$(date +%y%m%d)"

OBJDUMP = "${CROSS_DIR}/bin/${HOST_PREFIX}objdump -p"
OBJDUMP = "${CROSS_BIN_DIR}objdump -p"
LIB_LIST = "${S}/liblist"

# perform some patches to the rootfs
rootfs_postprocess() {
		# unlink kernel
		cp -Lr ${IMAGE_ROOTFS}/boot boot.tmp
		rm -rf ${IMAGE_ROOTFS}/boot boot.tmp/*-*
		mv boot.tmp ${IMAGE_ROOTFS}/boot

		# remove shipped modules
		for i in drivers/net/8139too.ko drivers/fsr drivers/xsr drivers/usb/host drivers/usb/core drivers/usb/storage fs/rfs* ; do 
			rm -rf ${IMAGE_ROOTFS}/lib/modules/*/kernel/${i} || true
		done
		install -d ${IMAGE_ROOTFS}/${MACHINE}/image
		# create partitions
		for i in u-boot.bin uboot_env.bin fnw.bin Image ; do
			oenote "create $i"
		done
		
		# redistribute crc ddcmp MicomCtrl ? (MicomCtrl from svn?)
		rm -f ${IMAGE_ROOTFS}/sbin/depmo* || true
		mv ${IMAGE_ROOTFS}/sbin/* ${IMAGE_ROOTFS}/${MACHINE}/ || true
		mv ${IMAGE_ROOTFS}/lib/modules ${IMAGE_ROOTFS}/${MACHINE}/
		
		# dd if=/dev/zero bs=1 count=${UBOOT_MAXSIZE} | tr '\0' '\377' > ${IMAGE_ROOTFS}/${MACHINE}/image/u-boot.bin
		# dd if=${DEPLOY_DIR_IMAGE}/${UBOOT_SYMLINK} of=${IMAGE_ROOTFS}/${MACHINE}/image/u-boot.bin conv=notrunc

		# original uImage has no comment (mkimage -n flag)? we can compress the kernel too
		dd if=/dev/zero bs=1 count=${KERNEL_IMAGE_MAXSIZE} | tr '\0' '\377' > ${IMAGE_ROOTFS}/${MACHINE}/image/Image
		dd if=${IMAGE_ROOTFS}/boot/${KERNEL_IMAGETYPE} of=${IMAGE_ROOTFS}/${MACHINE}/image/Image bs=64 skip=1 conv=notrunc
		
		# do the work for info.txt          serial_temp       validinfo.txt     version_info.txt
		# some CIP has major_version minor_version
		# image/validinfo.txt
		# cksfv -b ${IMAGE_ROOTFS}/${MACHINE}/image/*.bin -b ${IMAGE_ROOTFS}/${MACHINE}/image/Image > \
		# ${IMAGE_ROOTFS}/${MACHINE}/image/validinfo.txt
		cksfv -b ${IMAGE_ROOTFS}/${MACHINE}/image/Image > ${IMAGE_ROOTFS}/${MACHINE}/image/validinfo.txt

		# image/serial_temp -> 2009.0
		echo "${FW_VER}" > ${IMAGE_ROOTFS}/${MACHINE}/image/serial_temp
		printf "%s" ${FW_VER} > ${IMAGE_ROOTFS}/${MACHINE}/image/minor_version
		echo "${FW_MAJO}" > ${IMAGE_ROOTFS}/${MACHINE}/image/major_version

		# image/info.txt -> T-CHL5DEUC 2009.0
		echo "${MACHINE} ${FW_VER}" > ${IMAGE_ROOTFS}/${MACHINE}/image/info.txt	 

		# image/version_info.txt -> SWU_T-CHL5DEUC_002009_I02_ES000DS000_100126
		# SWU_T-VAL9DEUC_001001_I04_ES000DS000KS000RS000_100523 ???
		# EK000DK00 & ES000DS000 important?
		echo "SWU_${MACHINE}_${FW_MAJO}_I02_ES000DS000_${FW_DATE}" > ${IMAGE_ROOTFS}/${MACHINE}/image/version_info.txt

		echo 'for usage: unpack zip file to root of your usb pen.
        you should have 2 directories on your usb-pens root (boot and ${MACHINE})
before you flash the new kernel pls test it from u-boot
	turn off watchdog in service menu
        plug usb pen to tv
        enter u-boot promt at boot (menu 0)
        at LEONID prompt type:
                usb start;fatload usb 0 61000000 /boot/uImage;bootm
if your tv starts normal than you can flash the kernel:
        on turned on tv plug your usb-pen to tv, in popup window go to "software update" and follow instructions' > ${IMAGE_ROOTFS}/README
}

inherit image
