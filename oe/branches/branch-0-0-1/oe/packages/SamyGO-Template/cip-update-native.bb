require cip-update.bb

inherit native 

S = ${WORKDIR}/cip-update

do_compile () {
        oe_runmake
}

do_stage () {
        install -m 0755 encrypt_update ${STAGING_BINDIR}/
        install -m 0755 decrypt_update ${STAGING_BINDIR}/
}
