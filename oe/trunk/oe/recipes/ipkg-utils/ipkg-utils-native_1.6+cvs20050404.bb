require ipkg-utils_${PV}.bb

RDEPENDS = ""
PR = "r21"

inherit native

#SamyGO: added portability patch for ar
SRC_URI += "file://remove_f_from_ar_param.patch;patch=1"

# Avoid circular dependencies from package_ipk.bbclass
PACKAGES = ""
FILESDIR = "${@os.path.dirname(bb.data.getVar('FILE',d,1))}/ipkg-utils"
INSTALL += "arfile.py"

do_stage() {
        for i in ${INSTALL}; do
                install -m 0755 $i ${STAGING_BINDIR}
        done
}
