PACKAGES = task-samsung-static
DESCRIPTION = "Meta-package SamyGO (minimal enviroment with busybox-static)"
ALLOW_EMPTY = 1
PACKAGES = "${PN}"

PV = "0.0.1"
PR = "r0"

DEPENDS += "task-samsung-toolchain "
RDEPENDS = "base-files busybox-static "

