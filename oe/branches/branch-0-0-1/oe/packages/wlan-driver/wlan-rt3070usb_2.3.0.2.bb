DESCRIPTION = "Driver for Ralink RT307x USB 802.11b/g WiFi sticks"
HOMEPAGE = "http://www.ralinktech.com.tw"
SECTION = "kernel/modules"
LICENSE = "GPL"

# "http://www.ralinktech.com/download.php?t=U0wyRnpjMlYwY3k4eU1ERXdMekEwTHpFMUwyUnZkMjVzYjJGa09Ea3pOREU1TnpBeE9TNWllakk5UFQxRVVFOWZVbFF6TURjd1gweHBiblY0VTFSQlgxWXlMak11TUM0eVh6SXdNVEF3TkRFeUM="
# DPO_RT3070_LinuxSTA_V2.3.0.2_20100412.bz2 -> assholes -> DPO_RT3070_LinuxSTA_V2.3.0.2_20100412.tar.bz2

SRC_URI = "${RALINK_OS_MIRROR}/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412.tar.bz2 \
	file://selp-fix_stupidity.patch;patch=1 \
	file://selp-ver_string_size.patch;patch=1 \
	file://samygo-brand_modname.patch;patch=1 \
"

# RDEPEND = "wireless-tools"

KERNEL_MAJOR_VERSION = "2.6"

inherit module

S = "${WORKDIR}/DPO_RT3070_LinuxSTA_V2.3.0.2_20100412"
EXTRA_OEMAKE = "KERNDIR=${STAGING_KERNEL_DIR}"

do_unpack2() {
	echo "unpack2"
        # mv -f ${S}/Makefile.6 ${S}/Makefile
        # Branding
        # perl -pi -e "s/0.994/0.994 \[SamyGO\]/g" ${WORKDIR}/linux/linux-r011/drivers/rt73/Module/rtmp_main.c
        # perl -pi -e "s/1.002\(1.4.0.0\)/1.002\(1.4.0.0\) \[SamyGO\]/g" ${WORKDIR}/linux/linux-r011/drivers/rt2870/os/linux/2870_main_dev.c
        perl -pi -e "s/2.3.0.0/\[SamyGO\]/g" ${S}/include/os/rt_linux.h
}

addtask unpack2 before do_patch after do_unpack

#do_patch(){
#	may fixed in this version cp RT2870STA.dat RT3070STA.dat
#	usb ids at common/rtusb_dev_id.c
#	:
#}

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
        install -m 0644 os/linux/*.ko ${D}${base_libdir}/modules/${KERNEL_VERSION}/kernel/drivers/net/
        install -d ${D}/etc/Wireless/RT2870STA/
	install ${S}/RT2870STA.dat ${D}/etc/Wireless/RT2870STA/RT3070STA.dat
	install ${S}/RT2870STACard.dat ${D}/etc/Wireless/RT2870STA/
#        install -m 0644 rt71.bin ${D}/lib/firmware/
}

do_rm_work () {
        oenote "we keep data for now!"
        :
}

