require linux-chelsea-common_2.6.18.inc

SRC_URI += "file://defconfig_externalboot \
"

KERNEL_IMAGETYPE_LIST = "zImage uImage"

do_configure_prepend() {
	echo ${CROSS_COMPILE} > .mvl_cross_compile
	echo ${TARGET_ARCH} > .mvl_target_cpu
	cp ${WORKDIR}/defconfig_nfsboot .config
	make include/linux/version.h
}

