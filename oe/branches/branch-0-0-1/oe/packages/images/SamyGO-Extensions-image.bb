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
PR = "r05"
PROVIDES += " ${MACHINE}-${DISTRO_NAME}-Extensions-image "

######################################################################################
DEPENDS = " "
RDEPENDS += "${INSTALL_PACKAGES}" 
EXTRA_IMAGEDEPENDS = ""
######################################################################################

# INSTALL_PACKAGES = "task-samsung init-over-swf wlan-rt73 wlan-rt2870 \
# dropbare util-linux-mount util-linux-umount cksfv crypt-xor won't install 
# if RRECOMMENDS in InitScripts ?? * Arris *
INSTALL_PACKAGES = " \
task-samsung cksfv crypt-xor squashfs-tools StarterLib InitScripts \
# netsurf \
"

# BROKEN BUT NEEDED = libptp2
#
export IMAGE_BASENAME = "${DISTRO_NAME}-Extensions-image"
export IMAGE_LINGUAS = ""

export IPKG_INSTALL = "${INSTALL_PACKAGES}"

######################################################################################


USE_DEVFS = 1
export NFO = '${DEPLOY_DIR_IMAGE}/${IMAGE_NAME}.nfo'
export NFOT = '${DEPLOY_DIR_IMAGE}/${IMAGE_NAME}.desc'

IMAGE_FSTYPES = "zip"
IMAGE_CMD_zip = " \
#			fix SEC_GAME for some MACHINE
		cd ${IMAGE_ROOTFS}/mtd_tlib && zip ${EXTRA_IMAGECMD} \
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

OBJDUMP = "${CROSS_DIR}/bin/${HOST_PREFIX}objdump -p"
LIB_LIST = "${S}/liblist"
# perform some patches to the rootfs
rootfs_postprocess() {
	rm -rf ${IMAGE_ROOTFS}/usr/lib/ipkg
	rm -rf ${IMAGE_ROOTFS}/etc/terminfo
	rm -rf ${IMAGE_ROOTFS}/usr/share
	rm -rf ${IMAGE_ROOTFS}/var
	for j in ${IMAGE_ROOTFS}/lib/modules/2* ; do
		for i in $(find ${IMAGE_ROOTFS}/lib/modules/$(basename $j) -name "*.ko") ; do 
			mv $i ${IMAGE_ROOTFS}/lib/modules/ 
		done
	done
	rm -rf ${IMAGE_ROOTFS}/lib/modules/2*
	rm -f ${IMAGE_ROOTFS}/lib/modules/usbcore.ko || true
	mv ${IMAGE_ROOTFS}/mtd_rwarea/bluetooth ${IMAGE_ROOTFS}/etc/

	mkdir -p ${IMAGE_ROOTFS}/opt/privateer/lib
	mkdir -p ${IMAGE_ROOTFS}/opt/privateer/bin
	mkdir -p ${IMAGE_ROOTFS}/opt/privateer/sbin
	mkdir -p ${IMAGE_ROOTFS}/opt/privateer/usr/bin
	mkdir -p ${IMAGE_ROOTFS}/opt/privateer/usr/sbin
	echo "from ROOTFS_POSTPROCESS_COMMAND ignore libs ${IGNORED_LIBS}"
	echo > ${LIB_LIST}
	for i in ${IMAGE_ROOTFS}/bin/* ${IMAGE_ROOTFS}/sbin/* ${IMAGE_ROOTFS}/usr/bin/* ${IMAGE_ROOTFS}/usr/sbin/* ; do
		${OBJDUMP} $i | grep NEEDED | cut -d ' ' -f9- >> ${LIB_LIST}
		sort <${LIB_LIST} | uniq >${LIB_LIST}.sort
	done
	# try to find correct libs
	for j in `cat ${LIB_LIST}.sort` ; do
		for i in `find ${IMAGE_ROOTFS}/lib -name $j` `find ${IMAGE_ROOTFS}/usr/lib -name $j` ; do
			REAL=$(readlink -f $i)
			install ${REAL} ${IMAGE_ROOTFS}/opt/privateer/lib/$j && rm -vf $i
		done
	done
	for j in ${IGNORED_LIBS} ; do
		rm -fv ${IMAGE_ROOTFS}/opt/privateer/lib/$j || true
	done
	rm -fr ${IMAGE_ROOTFS}/lib/*.so* ${IMAGE_ROOTFS}/usr/lib*
	
	# try to find & dereference binaries (f... vfat)
	for j in /bin /usr/bin /sbin /usr/sbin ; do
		for i in `find ${IMAGE_ROOTFS}${j} -type l` ; do
			REAL=$(readlink -f $i)
			if [ "$(dirname $REAL)" = "/usr/bin" -o "$(dirname $REAL)" = "/usr/sbin" ] ; then
				echo "absolute link: install ${REAL} ${IMAGE_ROOTFS}/opt/privateer/${j}/$(basename $i) && rm -vf $i"
				install -m 755 ${IMAGE_ROOTFS}${REAL} ${IMAGE_ROOTFS}/opt/privateer/${j}/$(basename $i) && rm -vf $i
			else
				echo "relative link: install ${IMAGE_ROOTFS}${REAL} ${IMAGE_ROOTFS}/opt/privateer/${j}/$(basename $i) && rm -vf $i"
				install -m 755 ${REAL} ${IMAGE_ROOTFS}/opt/privateer/${j}/$(basename $i) && rm -vf $i
			fi
		done
	done

	date "+%m%d%H%M%Y" > ${IMAGE_ROOTFS}/etc/timestamp
	echo "privateer (${DISTRO_NAME}) $(date "+%m%d%H%M%Y")" > ${IMAGE_ROOTFS}/etc/release
	rm -f ${IMAGE_ROOTFS}/etc/init.d/[a-z]*
#	find ${IMAGE_ROOTFS} -name .debug -type d | xargs rm -rf
#	find ${IMAGE_ROOTFS} -name "*.util-linux" -type f | xargs rm -f 
	for j in ${IGNORED_APPS} ; do
		find ${IMAGE_ROOTFS} -name "${j}" | xargs rm -rvf
	done
	for j in ${DIST_APPS} ; do
		find ${IMAGE_ROOTFS} -name "${j}" -exec mv {} ${IMAGE_ROOTFS}/opt/privateer/usr/bin/ \;
	done
	for j in ${DIST_SAPPS} ; do
		find ${IMAGE_ROOTFS} -name "${j}" -exec mv {} ${IMAGE_ROOTFS}/opt/privateer/usr/sbin/ \;
	done
	rm -rf ${IMAGE_ROOTFS}/usr/sbin ${IMAGE_ROOTFS}/usr/bin ${IMAGE_ROOTFS}/etc/rc?.d
	for i in etc usr bin sbin opt lib ; do 
		cp -af ${IMAGE_ROOTFS}/$i ${IMAGE_ROOTFS}/mtd_tlib/${DISTRO_NAME}/ 
		rm -rf ${IMAGE_ROOTFS}/$i || true 
	done
	mv ${IMAGE_ROOTFS}/rcSGO ${IMAGE_ROOTFS}/mtd_tlib/${DISTRO_NAME}/
}
# fix some stuff from here? * Arris *
ROOTFS_POSTPROCESS_COMMAND += "rootfs_postprocess" 
# libs on tv
IGNORED_LIBS = "libc.so.6 libpthread.so.0 libgcc_s.so.1 libm.so.6 libdl.so.2 librt.so.1 libstdc++.so.6 libresolv.so.2"
IGNORED_APPS = "*.util-linux tz* .debug"
DIST_APPS = "${INSTALL_PACKAGES} djmount wakelan fusesmb fusesmb.cache showmount [sn]mb*"
DIST_SAPPS = "ulockmgr_server vsftpd"

