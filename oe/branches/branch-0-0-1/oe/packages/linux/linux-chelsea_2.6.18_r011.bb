DESCRIPTION = "Linux kernel for Samsung TV's T-CHL5DEUC, T-CHL5DAUC - used b550 sources"
HOMEPAGE = "http://www.samsung.com/global/opensource/files"
LICENSE = "GPL"

SRC_URI = "${SAMSUNG_OS_MIRROR}/LE32B550.zip \
# liar from samsung		${SAMSUNG_OS_MIRROR}/10_UE40B6000.zip \
		${SAMSUNG_OS_MIRROR}/32B650.zip \
		${SAMSUNG_OS_MIRROR}/LE40B650T2P.zip \
                file://${MACHINE}-dotconfig \
"
SRC_URI_append_samygo += "file://selp-gadget.patch;patch=1 \
                file://selp-ralink-devlist.patch;patch=1;pnum=0 \
                file://selp-ralink-devlist_2.2.0.0.patch;patch=1;pnum=0 \
                file://selp_experimantal_d-link_110.patch;patch=1 \
                file://selp-lgfb.patch;patch=1 \
#                file://selp-stupid-defines.patch;patch=1 \
#		http://www.fi.muni.cz/~adelton/linux/vfat-symlink/linux-2.4.13-vfat-symlink-0.90.patch;patch=1 \
"

S = "${WORKDIR}/linux/linux-r011"

require samygo-modules.inc
PR = "${INC_PR}.3.011"

DEFAULT_PREFERENCE = "-99"

KERNEL_VERSION_samygo = ${PV}${KERNEL_LOCALVERSION}

inherit kernel
DEPENDS = ""

FILES_dvb-includes = "/usr/include/linux/dvb"
FILES_scsi-includes = "/usr/include/linux/scsi"

COMPATIBLE_HOST = "arm.*-linux"
# Chelsea kernel
COMPATIBLE_MACHINE = "T-CHL5.*UC"
# COMPATIBLE_MACHINE_CLASS = "leonid"
DEFAULT_PREFERENCE_leonid = "11"

export OS = "Linux"
ARCH = "arm"

do_unpack2() {
        tar -xvzf ${WORKDIR}/linux_b678eu.tgz -C ${WORKDIR}/

	tar --strip-components=3 -xvzf ${WORKDIR}/linux.chelsea.tgz -C ${S}/ \
		linux/RFS/RFS_1.2.3p1_b015-XSR_1.5.2p4_b085_Auth-tinyBML_1.1.0p2_RTM_64bit_256MB

# rt drivers from cip sources
	
        tar -xvzf ${WORKDIR}/linux.cip.open.tgz -C ${WORKDIR}/ linux/linux-r011/drivers/rt73
        tar -xvzf ${WORKDIR}/linux.cip.open.tgz -C ${WORKDIR}/ linux/linux-r011/drivers/rt2870
	echo 'obj-y                           += rt2870/os/linux/
obj-y                           += rt73/Module/' >> ${S}/drivers/Makefile
        perl -pi -e "s/0.994/0.994 \[SamyGO\]/g" ${WORKDIR}/linux/linux-r011/drivers/rt73/Module/rtmp_main.c
        perl -pi -e "s/1.002\(1.4.0.0\)/1.002\(1.4.0.0\) \[SamyGO\]/g" ${WORKDIR}/linux/linux-r011/drivers/rt2870/os/linux/2870_main_dev.c

# xsr and rfs my not really required, but for fun.
	sed -i -e 's,^#\(source "drivers/xsr/Kconfig"\),\1,' ${S}/arch/arm/Kconfig
	sed -i -e 's,^#\(source "drivers/tbml/Kconfig"\),\1,' ${S}/arch/arm/Kconfig
	sed -i -e 's,^#\(obj-$(CONFIG_RFS_XSR).*+= xsr/\),\1,' ${S}/drivers/Makefile
	sed -i -e 's,^#\(obj-$(CONFIG_RFS_TINYBML).*+= tbml/\),\1,' ${S}/drivers/Makefile

	sed -i -e 's,^#\(source.*"fs/rfs\)\(128m\)\(/Kconfig"\),\1\3,' ${S}/fs/Kconfig
	sed -i -e 's,^#\(obj-$(CONFIG_RFS_FS).*+= rfs\)\(128m\)\(/\),\1\3,' ${S}/fs/Makefile

	sed -i -e 's,rfs128m,rfs,' ${S}/fs/rfs/Makefile

	pushd ${WORKDIR}
	  rm -f LPGL_smpeg.tar.gz			\
		selp_cross-binutils-2.17.50-10.0.0.tgz	\ 
		selp_glibc-2.5.90-9.0.9.tgz		\
		uboot_b650.tgz				\
		wireless_tools.28.tgz			\
		xfsprogs-3.0.5.0124.tgz			\
		busybox.B678.tgz			\
		ffmpeg.tgz				\
		LGPL_LIBGPHOTO2.tar.gz			\
		LGPL_LIBUSB.tar.gz			\
		LGPL_SDL.tar.gz				\
		linux_b678eu.tgz			\
		squashfs4.0.tar.gz  			\
			libgphoto2-2.3.1.tar.zip 	\
			libusb-0.1.12.tar.gz 		\
			linux.chelsea.tgz 		\
			SDL-1.2.11.zip 			\
			SELP.3.2.x-Chelsea.src.tgz	\
				linux.cip.open.tgz
	popd    
}

addtask unpack2 before do_patch after do_unpack

do_configure_prepend() {
	echo ${CROSS_COMPILE} > .mvl_cross_compile
	echo ${TARGET_ARCH} > .mvl_target_cpu

	rm -f .version || true
	echo '#define DTV_VERSION "SamyGO_128M_64_256"' > ${S}/include/asm-arm/arch-ssdtv/dtv_version.h	

	cp config.b678 .config || oe_machinstall -m 0644 ${WORKDIR}/${MACHINE}-dotconfig ${S}/.config
	oe_machinstall -m 0644 ${WORKDIR}/${MACHINE}-dotconfig ${S}/.config

	enable_modules
        # revert XFS
        perl -pi -e "s/(CONFIG_XFS_FS)=.*/# \1 is not set/" ${S}/.config
}

# one patch affects include/asm-arm/arch-ssdtv/memory-... may patch staged file too? 
do_stage_prepend () {
	rm ${S}/include/asm/arch-ssdtv || true
	cp -ar ${WORKDIR}/linux/ssdtv_platform/include/asm-arm/arch-ssdtv ${S}/include/asm/
	rm ${S}/include/asm-arm/arch || true
	ln -s arch-ssdtv ${S}/include/asm-arm/arch
}

do_stage_append () {
	install ${S}/.mvl_cross_compile ${STAGING_KERNEL_DIR}/ 
	install ${S}/.mvl_target_cpu ${STAGING_KERNEL_DIR}/
}

do_rm_work () {
        oenote "we keep data for now!"
        :
}
