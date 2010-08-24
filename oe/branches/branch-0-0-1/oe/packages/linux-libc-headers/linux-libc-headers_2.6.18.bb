require linux-libc-headers.inc

INHIBIT_DEFAULT_DEPS = "1"

DEPENDS += "unifdef-native"

INC_PR = "r13"
PR = "${INC_PR}.1"

DEFAULT_PREFERENCE = "-99"
DEFAULT_PREFERENCE_trihidtv = "10"

SRC_URI = "${SAMSUNG_OS_MIRROR}/LN40C550.zip"

S = "${WORKDIR}/mv_selp_kernel_1C_3B_RFS131_FSR120/kernel-2.6.18_pro500-mips24ke_nfp_be"

do_unpack2() {
        tar -xvf ${WORKDIR}/linux.tgz -C ${WORKDIR}/
        rm -f ${WORKDIR}/*.tar ${WORKDIR}/*.tgz ${WORKDIR}/*.gz
}

addtask unpack2 before do_patch after do_unpack

do_configure () {
	set_arch
	
	echo > .mvl_cross_compile
	echo ${ARCH} > .mvl_target_cpu
	oe_runmake allnoconfig ARCH=${ARCH}	
}

do_compile(){
	:
}

STAGE_TEMP="${WORKDIR}/temp-staging"

do_stage () {
        set_arch
        echo $ARCH
        rm -rf ${STAGE_TEMP}
        mkdir -p ${STAGE_TEMP}
        oe_runmake headers_install INSTALL_HDR_PATH=${STAGE_TEMP}${exec_prefix} ARCH=$ARCH

	install ${S}/include/asm/sgi*.h ${STAGE_TEMP}${includedir}/asm/
	install ${S}/include/linux/if_addr.h ${STAGE_TEMP}${includedir}/linux/

        if [ "$ARCH" = "arm" ]; then
                cp ${WORKDIR}/procinfo.h ${STAGE_TEMP}${includedir}/asm/
        fi
        install -d ${STAGING_INCDIR}
        rm -rf ${STAGING_INCDIR}/linux ${STAGING_INCDIR}/asm ${STAGING_INCDIR}/asm-generic
        cp -pfLR ${STAGE_TEMP}${includedir}/linux ${STAGING_INCDIR}/
        cp -pfLR ${STAGE_TEMP}${includedir}/asm ${STAGING_INCDIR}/
        cp -pfLR ${STAGE_TEMP}${includedir}/asm-generic ${STAGING_INCDIR}/
}

do_install() {
        set_arch
        oe_runmake headers_install INSTALL_HDR_PATH=${D}${exec_prefix} ARCH=${ARCH}
}

