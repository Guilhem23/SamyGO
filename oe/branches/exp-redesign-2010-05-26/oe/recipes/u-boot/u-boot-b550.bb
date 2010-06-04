require u-boot_b650.bb

DEFAULT_PREFERENCE = "-1"

SRC_URI += "file://noflash.patch;patch=1"

