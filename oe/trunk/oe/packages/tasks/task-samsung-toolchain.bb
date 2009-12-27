PACKAGES = task-samsung-toolchain
DESCRIPTION = "Meta-package to build Samsung SELP based Toolchain"
ALLOW_EMPTY = 1
PACKAGES = "${PN}"

PV = "0.0.1"
PR = "r0"

DEPENDS += "virtual/libc "
RDEPENDS = " "

COMPATIBLE_HOST = 'arm'
