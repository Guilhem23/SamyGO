DESCRIPTION = "Driver for Ralink rt73 USB 802.11b/g WiFi sticks"
HOMEPAGE = "http://www.ralinktech.com.tw"
SECTION = "kernel/modules"
LICENSE = "GPL"

# "http://www.ralinktech.com/download.php?t=U0wyRnpjMlYwY3k4eU1EQTVMekF6THpNeEwyUnZkMjVzYjJGa056YzNPREV4TlRjMk55NW5lajA5UFZKVU1qVlZVMEl0VTFKRExWWXlMakF1T0M0d0xuUmhD"
# RT25USB-SRC-V2.0.8.0.tar.gz (wrong package name -> RT25USB-SRC-V2.0.8.0.ta..gz)
# Firmware
# http://www.ralinktech.com/download.php?t=U0wyRnpjMlYwY3k4eU1EQTVMekF6THpNeEwyUnZkMjVzYjJGa056YzVPVE13TmpZMk9TNTZhWEE5UFQxU1ZEY3hWMTlHYVhKdGQyRnlaVjlXTVM0NEM%3D
# RT71W_Firmware_V1.8.zip

SRC_URI = "${RALINK_OS_MIRROR}/RT25USB-SRC-V2.0.8.0.tar.gz \
           file://rt25.make.patch;patch=1 \
           file://rt25_devname.patch;patch=1 "

# RDEPEND = "wireless-tools"

KERNEL_MAJOR_VERSION = "2.6"

inherit module

FILES_${PN} += " /lib/firmware/rt71.bin"

S = "${WORKDIR}/RT25USB-SRC-V2.0.8.0/"
EXTRA_OEMAKE = "KERNDIR=${STAGING_KERNEL_DIR}"

do_unpack2() {
	echo "unpack2"
        # mv -f ${S}/Makefile.6 ${S}/Makefile
        # Branding
        # perl -pi -e "s/0.994/0.994 \[SamyGO\]/g" ${WORKDIR}/linux/linux-r011/drivers/rt73/Module/rtmp_main.c
        # perl -pi -e "s/1.002\(1.4.0.0\)/1.002\(1.4.0.0\) \[SamyGO\]/g" ${WORKDIR}/linux/linux-r011/drivers/rt2870/os/linux/2870_main_dev.c
}

addtask unpack2 before do_patch after do_unpack

do_compile () {
        unset CFLAGS CPPFLAGS CXXFLAGS LDFLAGS CC LD CPP
        oe_runmake 'MODPATH={D}${base_libdir}/modules/${KERNEL_VERSION}/kernel/drivers/net' \
                   'KERNEL_SOURCE=${STAGING_KERNEL_DIR}' \
                   'LINUX_SRC=${STAGING_KERNEL_DIR}' \
                   'KDIR=${STAGING_KERNEL_DIR}' \
                   'KERNDIR=${STAGING_KERNEL_DIR}' \
                   'KERNEL_VERSION=${KERNEL_VERSION}' \
                   'CC=${KERNEL_CC}' \
                   'LD=${KERNEL_LD}'
}

do_install() {
        install -d ${D}${base_libdir}/modules/${KERNEL_VERSION}/kernel/drivers/net
        install -m 0644 *.ko ${D}${base_libdir}/modules/${KERNEL_VERSION}/kernel/drivers/net/
#        install -d ${D}/lib/firmware
#        install -m 0644 rt71.bin ${D}/lib/firmware/
}

