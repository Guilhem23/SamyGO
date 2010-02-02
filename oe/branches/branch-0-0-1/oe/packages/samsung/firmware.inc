DESCRIPTION = "Samsung Firmware"
HOMEPAGE = "http://www.samsung.com"
SECTION = "Firmware"
PRIORITY = "required"
LICENSE = "Proprietary"

# sure is sure
# DEPENDS += " coreutils-native crypt-xor-native cksfv-native "
DEPENDS += " crypt-xor-native cksfv-native "

inherit autotools

FILES = "${MACHINE}"
SRC_URI = "http://www.samsung.com/download/${FILES}-${PV}.exe"

FETCHCOMMAND_wget = "wget ${DL_URI} -O ${DL_DIR}/${FILES}-${PV}.exe"
