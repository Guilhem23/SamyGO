#
# Copyright Ser Lev Arris <arris@ZsoltTech.Com> (c) 2009-2010
# License: GPL (see http://www.gnu.org/licenses/gpl.txt for a copy of the license)
#
# Filename: ${DISTRO_NAME}-Extensions-image.bb 
# Date: 06-Jan-2010

DESCRIPTION = "<description>"
MAINTAINER = "Ser Lev Arris <arris@ZsoltTech.Com>"
HOMEPAGE = "http://stb.ZsoltTech.Com/"

######################################################################################

PV = "0.0.1"
PR = "r01"
PROVIDES += " ${MACHINE}-${DISTRO_NAME}-PatchedFirmware-image "

######################################################################################
DEPENDS = " "
RDEPENDS += "${INSTALL_PACKAGES}" 
EXTRA_IMAGEDEPENDS = ""
######################################################################################

# INSTALL_PACKAGES = "task-samsung init-over-swf wlan-rt73 wlan-rt2870 \
# dropbare util-linux-mount util-linux-umount cksfv crypt-xor won't install 
# if RRECOMMENDS in InitScripts ?? * Arris *
INSTALL_PACKAGES = " \
samsung-firmware \
"

# BROKEN BUT NEEDED = libptp2
#
export IMAGE_BASENAME = "${DISTRO_NAME}-PatchedFirmware-image"
export IMAGE_LINGUAS = ""

export IPKG_INSTALL = "${INSTALL_PACKAGES}"

######################################################################################


USE_DEVFS = 1
export NFO = '${DEPLOY_DIR_IMAGE}/${IMAGE_NAME}.nfo'
export NFOT = '${DEPLOY_DIR_IMAGE}/${IMAGE_NAME}.desc'

# fix some stuff from here? * Arris *
# ROOTFS_POSTPROCESS_COMMAND = "" 

IMAGE_FSTYPES = "zip"
IMAGE_CMD_zip = " \
		rm -rf ${IMAGE_ROOTFS}/usr ; \
		rm -rf ${IMAGE_ROOTFS}/dev ; \
		rm -rf ${IMAGE_ROOTFS}/etc/ipkg ; \
		cd ${IMAGE_ROOTFS} && zip ${EXTRA_IMAGECMD} \
		${DEPLOY_DIR_IMAGE}/${IMAGE_NAME}.zip ."

EXTRA_IMAGECMD_zip = "-r"

do_rootfs_append() {
        if [ "${VER}" != "" ]; then
                printf "exeDSP: Experimental ${VER}\n" > ${NFO}
        else
               printf "exeDSP: Unknown\n" > ${NFO}
        fi
        printf "Machine: Samsung ${MACHINE}\n" >> ${NFO}
        DATE=`date +%Y-%m-%d' '%H':'%M`
        printf "Date: ${DATE}\n" >> ${NFO}
        printf "Issuer: ZsoltTech.Com\n" >> ${NFO}
        VER=`echo ${DISTRO_VERSION} | cut -b 1-3`
        # printf "Link: ${FEED_URIS}\n" >> ${NFO}
        printf "Link: ${FEED_URIS}\n" >> ${NFO}
        if [ "${DESC}" != "" ]; then
                printf "Description: ${DESC}\n" >> ${NFO}
                printf "${DESC}\n" >> ${NFO}
        fi
        MD5SUM=`md5sum ${DEPLOY_DIR_IMAGE}/${IMAGE_NAME}.zip | cut -b 1-32`
        printf "MD5: ${MD5SUM}\n" >> ${NFO}
}

inherit image_ipk

