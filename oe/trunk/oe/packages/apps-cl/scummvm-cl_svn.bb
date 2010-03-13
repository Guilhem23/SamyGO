require scummvm-cl.inc

CCACHE = ""

DEPENDS = "libsdl-cl libmad-cl flac-cl tremolo-cl fluidsynth-cl zlib"

SRC_URI += " \
           "

S = "${WORKDIR}/trunk"

SRC_URI_OVERRIDES_PACKAGE_ARCH = "1"

EXTRA_OECONF += " --host=samsungtv --enable-release --disable-debug \
		"

# Workaround, because some env variables aren't recognised correctly
do_configure_append() {
	sed -i "s/AS := as/AS := ${AS}/" ${S}/config.mk
	sed -i "s/AR := ar cru/AR := ${AR} cru/" ${S}/config.mk
	sed -i "s/STRIP := strip/STRIP := ${STRIP}/" ${S}/config.mk
	sed -i "s/RANLIB := ranlib/RANLIB := ${RANLIB}/" ${S}/config.mk
}

do_install_append() {
	if  [ -f ${WORKDIR}/scummvm.desktop ]; then
		install -d ${D}${datadir}/applications
#		install -m 0644 ${WORKDIR}/scummvm.desktop ${D}${datadir}/applications
	fi
}
