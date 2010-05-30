DESCRIPTION = "Driver for DM9601 chip based USB/Ethernet adapter (Sunrising USB Ethernet adapter)"
HOMEPAGE = "http://www.davicom.com.tw"
SECTION = "kernel/modules"
LICENSE = "GPL"


# may original driver at http://www.meworks.net/userfile/24247/dm9601-Linux2.6_1.tar
# http://ubuntuforums.org/attachment.php?attachmentid=78751&d=1216876426
# DM9601.zip -> dm9601-2.6.tgz hate forums what not accept tgz files! 

SRC_URI = "${DAVICOM_OS_MIRROR}/DM9601.zip \
	file://selp-devids.patch;patch=1 \
"

# RDEPEND = "wireless-tools"

KERNEL_MAJOR_VERSION = "2.6"

inherit module

RDEPENDS = ""
PR = "r01"
# KERNEL_VERSION_append = "${KERNEL_LOCALVERSION}"

S = "${WORKDIR}/dm9601-2.6"
EXTRA_OEMAKE = 'KERNDIR="${STAGING_KERNEL_DIR}" KDIR="${STAGING_KERNEL_DIR}" KERNEL_SOURCE="${STAGING_KERNEL_DIR}"'

do_unpack2() {
	tar xvzf ${WORKDIR}/dm9601-2.6.tgz -C ${WORKDIR}/ && rm -f ${WORKDIR}/dm9601-2.6.tgz
	perl -pi -e "s/\r\n/\n/g" ${S}/dm9601.c ${S}/dm9601.h
}

addtask unpack2 before do_patch after do_unpack

#do_patch(){
#	may for new driver? (included in kernel after 2.6.21)
#	http://git.kernel.org/?p=linux/kernel/git/torvalds/linux-2.6.git;a=commitdiff;\
#		h=23de559b8d3537f972f325e7e015c18aa2e4d987	
#	adapted as patch (add two devids)
#	http://git.kernel.org/?p=linux/kernel/git/torvalds/linux-2.6.git;a=commitdiff;\
#		h=6dc477f3e4dd456122fdb231a1c57393659b93b5;hp=521b85ae66ad40a670b27b0596312d347a89b669
#	adapted as patch (add one devids)
#	http://21500.net/?tag=sr9600
#	
#}

do_install() {
        install -d ${D}${base_libdir}/modules/${KERNEL_VERSION}/kernel/drivers/net
        install -m 0644 *.ko ${D}${base_libdir}/modules/${KERNEL_VERSION}/kernel/drivers/net/
}

do_rm_work () {
        oenote "we keep data for now!"
        :
}

