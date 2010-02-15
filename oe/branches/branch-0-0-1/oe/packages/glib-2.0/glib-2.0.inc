USE_NLS_samygo = "yes"

export ac_cv_func_posix_getgrgid_r=yes

python () {
    import bb
    if bb.data.getVar("USE_NLS", d, 1) == "no":
        raise bb.parse.SkipPackage("${PN} requires native language support.")
}
