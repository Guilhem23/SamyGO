#
# Copyright Ser Lev Arris <arris@ZsoltTech.Com> (c) 2009
# License: GPL (see http://www.gnu.org/licenses/gpl.txt for a copy of the license)
#
# Filename: T-CHU7DEUC-image.bb
# Date: 23-Dec-09

DESCRIPTION = "<description>"
MAINTAINER = "Ser Lev Arris <arris@ZsoltTech.Com>"
HOMEPAGE = "http://stb.ZsoltTech.Com/"

PROVIDES += "T-SPHAUSC-static-image"

######################################################################################

PV = "0.0.1"
PR = "r0"

######################################################################################
DEPENDS += " "
RDEPENDS += "${INSTALL_PACKAGES}" 
######################################################################################

# INSTALL_PACKAGES = "task-samsung init-over-swf wlan-rt73 wlan-rt2870 \
INSTALL_PACKAGES = " \
task-samsung-static \
"

# BROKEN BUT NEEDED = libptp2
#
export IMAGE_BASENAME = "T-CHU7DEUC-static-image"
export IMAGE_LINGUAS = " "

export IPKG_INSTALL = "${INSTALL_PACKAGES}"

######################################################################################


USE_DEVFS = 1
export NFO = '${DEPLOY_DIR_IMAGE}/${IMAGE_NAME}.nfo'
export NFOT = '${DEPLOY_DIR_IMAGE}/${IMAGE_NAME}.desc'

# IMAGE_FSTYPES = "tar.gz squashfs squashfs-lzma"
IMAGE_FSTYPES = "tar.gz"
# export IMAGE_FSTYPES = "tar.gz"
IMAGE_CMD_tar.gz = "cd ${IMAGE_ROOTFS} && tar ${EXTRA_IMAGECMD} -zcvf ${DEPLOY_DIR_IMAGE}/${IMAGE_NAME}.tar.gz ."
# EXTRA_IMAGECMD_tar.gz = "--remove-files"

do_rootfs_append() {
#        VER=`< ${IMAGE_ROOTFS}/${MACHINE}/image/version_info.txt`
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
        printf "Link: ${FEED_URIS}\n" >> ${NFO}
        if [ "${DESC}" != "" ]; then
                printf "Description: ${DESC}\n" >> ${NFO}
                printf "${DESC}\n" >> ${NFO}
        fi
        MD5SUM=`md5sum ${DEPLOY_DIR_IMAGE}/${IMAGE_NAME}.fw2 | cut -b 1-32`
        printf "MD5: ${MD5SUM}\n" >> ${NFO}
        printf "\n######################\nPaket Liste:\n" >> ${NFOT}
        for i in ${IMAGE_ROOTFS}/usr/lib/ipkg/info/*.control ; do
                grep Package: $i >> ${NFOT}
                grep Description: $i | cut -b 14- >> ${NFOT}
                grep Version: $i >> ${NFOT}
                cat ${IMAGE_ROOTFS}/usr/lib/ipkg/info/$(basename $i .control).list >> ${NFOT}
		printf "\n" >> ${NFOT}
#		tree ${IMAGE_ROOTFS} >> ${NFOT}
#                printf "\n" >> ${NFOT}
       done
}
inherit image_ipk

