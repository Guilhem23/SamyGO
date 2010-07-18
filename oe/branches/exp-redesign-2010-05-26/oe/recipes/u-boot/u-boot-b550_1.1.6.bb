require u-boot_1.1.6.bb

DEFAULT_PREFERENCE = "-1"

SRC_URI += "file://b550.patch;patch=1 "

S = ${WORKDIR}/u-boot-1.1.6/

