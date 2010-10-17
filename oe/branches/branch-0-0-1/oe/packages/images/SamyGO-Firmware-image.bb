# samygo-extensions-image 
#
# Image configuration for the SamyGO Extensions Distribuion 
#
#
# Copyright Ser Lev Arris <arris@ZsoltTech.Com> (c) 2009-2010
# License: GPL (see http://www.gnu.org/licenses/gpl.txt for a copy of the license)
#
# Filename: ${DISTRO_NAME}-Extensions-image.bb
# Date: 04-Mai-2010 
# $Id: SamyGO-All-Extensions_v0.03.6.bb 1037 2010-10-13 16:43:22Z arris $

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
        ${DEPLOY_DIR_IMAGE}/${IMAGE_NAME}.zip ${MACHINE}"
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

OBJDUMP = "${CROSS_DIR}/bin/${HOST_PREFIX}objdump -p"
OBJDUMP = "${CROSS_BIN_DIR}objdump -p"
LIB_LIST = "${S}/liblist"

# perform some patches to the rootfs
rootfs_postprocess() {
		install -d ${IMAGE_ROOTFS}/${MACHINE}/image
		# redistribute crc ddcmp MicomCtrl ? (MicomCtrl from svn?)
		# create partitions
		for i in u-boot.bin uboot_env.bin fnw.bin Image ; do
			oenote "create $i"
		done
		
		rm -f ${IMAGE_ROOTFS}/sbin/depmo* || true
		mv ${IMAGE_ROOTFS}/sbin/* ${IMAGE_ROOTFS}/${MACHINE}/ || true
		mv ${IMAGE_ROOTFS}/lib/modules ${IMAGE_ROOTFS}/${MACHINE}/
		
		# dd if=/dev/zero bs=1 count=${UBOOT_MAXSIZE} | tr '\0' '\377' > ${IMAGE_ROOTFS}/${MACHINE}/image/u-boot.bin
		# dd if=${DEPLOY_DIR_IMAGE}/${UBOOT_SYMLINK} of=${IMAGE_ROOTFS}/${MACHINE}/image/u-boot.bin conv=notrunc

		# original uImage has no comment (mkimage -n flag)? can we compress the kernel?
		dd if=/dev/zero bs=1 count=${KERNEL_IMAGE_MAXSIZE} | tr '\0' '\377' > ${IMAGE_ROOTFS}/${MACHINE}/image/Image
		dd if=${IMAGE_ROOTFS}/boot/uImage-2.6.18_SELP-ARM of=${IMAGE_ROOTFS}/${MACHINE}/image/Image bs=64 skip=1 conv=notrunc

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
		echo "SWU_${MACHINE}_${FW_MAJO}_I02_ES000DS000_100126" > ${IMAGE_ROOTFS}/${MACHINE}/image/version_info.txt
}

inherit image
