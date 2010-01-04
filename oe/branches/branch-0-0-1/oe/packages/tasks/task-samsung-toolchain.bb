PACKAGES = task-samsung-toolchain
DESCRIPTION = "Meta-package to build Samsung SELP based Toolchain"
MAINTAINER = "Ser Lev Arris <arris@ZsoltTech.Com>"
ALLOW_EMPTY = 1
PACKAGES = "${PN}"

PV = "0.0.1"
PR = "r0"

RDEPENDS = " "

LICENSE = GPL

# there is a -march=586 somewhere in the source tree of mythtv
# COMPATIBLE_HOST = 'arm'

CORE_EXTRA_D ?= "virtual/libc"
CORE_EXTRA_RD ?= ""

CORE_PACKAGES_D = "${CORE_EXTRA_D}"

CORE_PACKAGES_RD = "${CORE_EXTRA_RD}"

######################################################################################

DEPENDS += "${CORE_PACKAGES_D}"
RDEPENDS += "${CORE_PACKAGES_RD}"

