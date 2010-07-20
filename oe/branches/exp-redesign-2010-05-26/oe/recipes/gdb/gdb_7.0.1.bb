require gdb.inc
LICENSE = "GPLv3"

SRC_URI += "\
            file://gdb-6.8-fix-compile-karmic.patch"

# Work-around problems while creating libbfd.a
EXTRA_OECONF += "--enable-static"

SRC_URI[md5sum] = "100d6e5524be1a34ba61556a7389a297"
SRC_URI[sha256sum] = "aa4c236316540250b322037ee88fda68499324b096cf34e75b591514c1d5bf7f"
