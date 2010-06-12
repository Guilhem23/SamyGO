require u-boot.inc

PR = "r0"

DEPENDS = ""

SRC_URI = "ftp://ftp.denx.de/pub/u-boot/u-boot-1.1.6.tar.bz2 \
	file://common-fixes.patch;patch=1 \
	file://fat-enchance.patch;patch=1 \
	file://usb-enchance.patch;patch=1 \
	file://keypress.patch;patch=1 \
	file://sdtvchl2.patch;patch=1 \
"

SRC_URI[md5sum] = "5b1b1f7b3b1e06f75f5bfbd79891067b"
SRC_URI[sha256sum] = "778acb0eafe1d9b94c6f5ec5f333126c40d73704920ff8b23085c6dedecfd6e8"

do_compile () {
        unset LDFLAGS
        unset CFLAGS
        unset CPPFLAGS
        oe_runmake sdtvchl2_config
        oe_runmake all
        oe_runmake tools
}

