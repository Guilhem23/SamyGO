require xorg-xserver-common.inc

DESCRIPTION = "the X.Org X server"
DEPENDS += "pixman libpciaccess openssl dri2proto glproto xorg-minimal-fonts font-util-native"
PE = "2"
PR = "${INC_PR}.0"

SRC_URI += " \
            file://dolt-fix-1.7.0.patch;patch=1 \
            file://randr-support-1.7.0.patch;patch=1 \
	    file://hack-fbdev-ignore-return-mode.patch;patch=1 \
	    file://fix-x86emu.patch;patch=1 \
           "
do_install_prepend() {
        mkdir -p ${D}/${libdir}/X11/fonts
}

# The NVidia driver requires Xinerama support in the X server. Ion uses it.
XINERAMA = "${@['--disable-xinerama','--enable-xinerama'][bb.data.getVar('MACHINE',d) in ['ion']]}"

EXTRA_OECONF += " ${CONFIG_MANAGER_OPTION} ${XINERAMA} --disable-kdrive --disable-xephyr --disable-xsdl --disable-xfake --disable-xfbdev --disable-dmx"
EXTRA_OECONF += " --disable-glx-tls --disable-dri --disable-unit-tests "

export LDFLAGS += " -ldl "
