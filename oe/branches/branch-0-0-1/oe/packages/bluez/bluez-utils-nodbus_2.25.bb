require bluez-utils.inc

PR = "r1"

DEPENDS = "bluez-libs-${PV}"

SRC_URI += " file://hciattach-ti-bts.patch;patch=1"
EXTRA_OECONF += "--without-dbus"
