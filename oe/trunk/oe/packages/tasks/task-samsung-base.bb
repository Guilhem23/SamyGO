PACKAGES = task-samsung-base
DESCRIPTION = "Meta-package Samsung SELP (minimal enviroment with busybox)"
ALLOW_EMPTY = 1
PACKAGES = "${PN}"

PV = "0.0.1"
PR = "r0"

DEPENDS += "task-samsung-toolchain "
RDEPENDS = "base-files busybox "
