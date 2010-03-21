DESCRIPTION = "Samsung Firmware"
HOMEPAGE = "http://www.samsung.com"
SECTION = "Firmware"
PRIORITY = "required"
LICENSE = "Proprietary"

# sure is sure
# DEPENDS += " coreutils-native crypt-xor-native cksfv-native "
DEPENDS += " crypt-xor-native cksfv-native squashfs-tools-native"

INHIBIT_PACKAGE_STRIP = "1"

inherit autotools

FILES = "${MACHINE}"

# if firmisrar/zip not included default to zip
REALEXT ?= "zip"

SRC_URI = "http://www.samsung.com/download/${FILES}-${PV}.${REALEXT}"

FETCHCOMMAND_wget = "wget ${DL_URI} -O ${DL_DIR}/${FILES}-${PV}.${REALEXT}"

S = "${WORKDIR}"
