PACKAGES = task-samsung-static
DESCRIPTION = "Meta-package Samsung SELP (minimal enviroment with busybox-static)"
MAINTAINER = "Ser Lev Arris <arris@ZsoltTech.Com>"
ALLOW_EMPTY = 1
PACKAGES = "${PN}"

PV = "0.0.1"
PR = "r0"

RDEPENDS = " "

LICENSE = GPL

# there is a -march=586 somewhere in the source tree of mythtv
# COMPATIBLE_HOST = 'arm'

CORE_EXTRA_D ?= "task-samsung-toolchain"
CORE_EXTRA_RD ?= " base-files busybox-static "

CORE_PACKAGES_D = "${CORE_EXTRA_D}"

CORE_PACKAGES_RD = "${CORE_EXTRA_RD}"

######################################################################################

DEPENDS += "${CORE_PACKAGES_D}"
RDEPENDS += "${CORE_PACKAGES_RD}"

