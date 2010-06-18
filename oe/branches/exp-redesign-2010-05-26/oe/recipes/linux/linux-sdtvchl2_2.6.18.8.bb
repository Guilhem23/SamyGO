DESCRIPTION = "Linux Kernel for Samsung TV's"
LICENSE = "GPLv2"
DEPENDS = "binutils-cross u-boot-mkimage-native"

# SamyGO: workaround samsung download sorces
#FETCHCOMMAND_wget = "/usr/bin/env 'PATH=${PATH}' wget -t 5 --passive-ftp --no-check-certificate -P ${DL_DIR} \
#	-O LA46B650.zip http://opensource.samsung.com/tv_n_video/la46b650/"
#RESUMECOMMAND_wget = "/usr/bin/env 'PATH=${PATH}' wget -c -t 5 --passive-ftp --no-check-certificate -P ${DL_DIR} \
#	-O LA46B650.zip http://opensource.samsung.com/tv_n_video/la46b650/"

SRC_URI = "${KERNELORG_MIRROR}/pub/linux/kernel/v2.6/linux-${PV}.tar.bz2 \
	file://fix_nonlinux_compile.patch;patch=1 \
	file://ignore-mod-magic-version.patch;patch=1 \
	file://defconfig_sdtvchl2 \
"

#http://opensource.samsung.com/tv_n_video/la46b650/LA46B650.zip \
#	file://fix_nonlinux_compile.patch;patch=1 \
#	file://gadget.patch;patch=1 \
#	file://ralink-devlist.patch;patch=1;pnum=0 \
#	file://ralink-devlist_2.2.0.0.patch;patch=1;pnum=0 \
#	file://pegasus.patch;patch=1 \
#	file://ignore-mod-magic-version.patch;patch=1 \
#	file://defconfig_sdtvchl2 \
#	file://Makefile.modinst \
#	file://Makefile.modpost \

SRC_URI[md5sum] = "dce47badc1faf34b355a10b97ae5d391"
SRC_URI[sha256sum] = "945b3014f8048cd87fdff90014afa4ff241f134bceafbfdbd42dba1be8df2ba8"

S = "${WORKDIR}/linux/linux-2.6.18.8"

KERNEL_VERSION = "2.6.18.8"
inherit kernel siteinfo

#FILES_dvb-includes = "/usr/include/linux/dvb"
#FILES_scsi-includes = "/usr/include/linux/scsi"

COMPATIBLE_HOST = "arm.*-linux"
COMPATIBLE_MACHINE = "sdtvchl2"

export ARCH = "arm"
export OS = "Linux"

UBOOT_ENTRYPOINT = "0x60008000"

do_install_append() {
	oe_runmake headers_install INSTALL_HDR_PATH=${D}${exec_prefix}/src/linux-${KERNEL_VERSION} ARCH=$ARCH
}

PACKAGES =+ "kernel-headers"
FILES_kernel-headers = "${exec_prefix}/src/linux*"

#do_unpack2() {
#	tar -xvzf ${WORKDIR}/linux.cip.open.tgz -C ${WORKDIR}/
#	rm -f ${WORKDIR}/*.zip ${WORKDIR}/SELP* ${WORKDIR}/*.tgz ${WORKDIR}/*.gz ${WORKDIR}/README.txt || true
#	perl -pi -e "s/0.994/0.994 \[SamyGO\]/g" ${WORKDIR}/linux/linux-r011/drivers/rt73/Module/rtmp_main.c
#	perl -pi -e "s/1.002\(1.4.0.0\)/1.002\(1.4.0.0\) \[SamyGO\]/g" ${WORKDIR}/linux/linux-r011/drivers/rt2870/os/linux/2870_main_dev.c
#	echo "#define UTS_RELEASE \"2.6.18\"" > ${WORKDIR}/linux/linux-r011/include/linux/utsrelease.h
#}

#addtask unpack2 before do_patch after do_unpack

do_configure_prepend () {
#	echo ${CROSS_COMPILE} > .mvl_cross_compile
#	echo ${TARGET_ARCH} > .mvl_target_cpu
#	echo > localversion
#	echo > localversion-mvl
	cp ${WORKDIR}/defconfig_sdtvchl2 .config
#	cp ${WORKDIR}/Makefile.* scripts/
	make include/linux/version.h
}

KERNEL_IMAGETYPE = "uImage"

