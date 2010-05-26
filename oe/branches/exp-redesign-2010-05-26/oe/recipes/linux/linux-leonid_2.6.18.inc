DESCRIPTION = "Linux kernel for Samsung TV's"
HOMEPAGE = "http://www.samsung.com/global/opensource/files"
LICENSE = "GPL"
DEPENDS = "binutils-cross u-boot-mkimage-selp-native"

SRC_URI = "${SAMSUNG_OS_MIRROR}/32B650.zip;name=archive \
	${SAMSUNG_OS_MIRROR}/LE40B650T2P.zip;name=archive2 \
	file://selp-fix_nonlinux_compile.patch;patch=1 \
	file://selp-gadget.patch;patch=1 \
	file://selp-ralink-devlist.patch;patch=1;pnum=0 \
	file://selp-ralink-devlist_2.2.0.0.patch;patch=1;pnum=0 \
	file://pegasus.patch;patch=1 \
	file://ignore-mod-magic-version.patch;patch=1 \
	file://${KERNEL_DEFCONFIG} \
"

SRC_URI[archive.md5sum] = "232c2397d511253a8d6dcf66fa8d9bc2"
SRC_URI[archive.sha256sum] = "78cecf54357d014450355c35af21d2ad330c782a89f56fead6a65b238dbff0f6"
SRC_URI[archive2.md5sum] = "911a26848446e8b1af5380df51d0b5cb"
SRC_URI[archive2.sha256sum] = "8446af033be3c3569e240a20a5021cebdef70a019e9b20c6bae0d483b4d4515c"

S = "${WORKDIR}/linux/linux-r011"

KERNEL_VERSION = "2.6.18"
inherit kernel

FILES_dvb-includes = "/usr/include/linux/dvb"
FILES_scsi-includes = "/usr/include/linux/scsi"

COMPATIBLE_HOST = "arm.*-linux"
COMPATIBLE_MACHINE = "leonid"

DEFAULT_PREFERENCE = "-1"

export ARCH = "arm"
export OS = "Linux"

do_unpack2() {
	tar -xvzf ${WORKDIR}/linux.chelsea.tgz -C ${WORKDIR}/
	tar -xvzf ${WORKDIR}/linux.cip.open.tgz -C ${WORKDIR}/ linux/linux-r011/drivers/rt73
	tar -xvzf ${WORKDIR}/linux.cip.open.tgz -C ${WORKDIR}/ linux/linux-r011/drivers/rt2870
	rm -f ${WORKDIR}/*.zip ${WORKDIR}/SELP* ${WORKDIR}/*.tgz ${WORKDIR}/*.gz || true
	# Branding
	perl -pi -e "s/0.994/0.994 \[SamyGO\]/g" ${WORKDIR}/linux/linux-r011/drivers/rt73/Module/rtmp_main.c
	perl -pi -e "s/1.002\(1.4.0.0\)/1.002\(1.4.0.0\) \[SamyGO\]/g" ${WORKDIR}/linux/linux-r011/drivers/rt2870/os/linux/2870_main_dev.c
	echo "#define UTS_RELEASE \"2.6.18\"" > ${WORKDIR}/linux/linux-r011/include/linux/utsrelease.h
}

addtask unpack2 before do_patch after do_unpack

do_configure_prepend () {
	echo ${CROSS_COMPILE} > .mvl_cross_compile
	echo ${TARGET_ARCH} > .mvl_target_cpu
	echo > localversion
	echo > localversion-mvl
	cp ${WORKDIR}/${KERNEL_DEFCONFIG} .config
	make include/linux/version.h
}

KERNEL_IMAGETYPE_LIST = "Image uImage"

