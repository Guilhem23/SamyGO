DESCRIPTION = "SMPEG is a general purpose MPEG video/audio \
player for Linux based on the mpeg_play and SPLAY MPEG decoders."
LICENSE = "LGPL"
SECTION = "libs/multimedia"
DEPENDS = "virtual/libsdl"
PROVIDES = "smpeg"
# PV = "0.4.5+svnr${SRCREV}"
PE = "1"
SRCREV = "370"

SRC_URI = "${SAMSUNG_OS_MIRROR}/smpeg-0.4.5.zip \
	file://smpeg-config \
"

S = "${WORKDIR}/"

inherit autotools

export SDL_CONFIG = "${STAGING_BINDIR}/sdl-config-${HOST_SYS}"

CFLAGS_append = " -I${STAGING_INCDIR}/SDL"
EXTRA_OECONF = "--disable-gtktest --disable-opengl-player --without-x \
		--without-gtk --disable-gtk-player"

do_configure() {
	perl -pi -e "s/CROSS_COMPILE = (.*)/CROSS_COMPILE \?= \1/" Makefile*
	mkdir lib_A1	
}

do_compile_arm(){
	oe_runmake CROSS_COMPILE=${TARGET_PREFIX} A1
}

do_install_arm(){
	mkdir -p ${D}/lib
	mkdir -p ${D}/include
	cp -a lib_A1/libsmpeg.so ${D}/lib/
	cp -a include/smpeg ${D}/include/
}

do_configure_prepend_sh4() {
        mkdir lib_STI
	perl -pi -e "s/INC =  (.*)/INC \?= \1/" Makefile*
        oe_runmake cSTI
}

do_compile_sh4(){
	oe_runmake -f Makefile.STI CROSS_COMPILE=${TARGET_PREFIX} INC=-I${STAGING_INCDIR}/SDL all 
}

do_install_sh4(){
	mkdir -p ${D}/lib
	mkdir -p ${D}/include
	cp -a lib_STI/libsmpeg.so ${D}/lib/
	cp -a include/smpeg ${D}/include/
}

do_stage() {
	oe_libinstall -so -C ${D}/lib libsmpeg ${STAGING_LIBDIR}

	cp -a ${D}/include/smpeg ${STAGING_INCDIR}/	

echo "#!/bin/sh

prefix=${STAGING_LIBDIR}/..
exec_prefix=${STAGING_LIBDIR}/..
exec_prefix_set=no
" >> smpeg-config.tmp
        cat smpeg-config >> smpeg-config.tmp
        mv smpeg-config.tmp smpeg-config
        install -m 755 smpeg-config ${STAGING_BINDIR}/smpeg-config-${HOST_SYS}
}

PACKAGES =+ "plaympeg "
SECTION_plaympeg = "console/multimedia"
FILES_${PN} = "${libdir}"
FILES_plaympeg = "${bindir}/plaympeg"
FILES_${PN}-dev += "${bindir}"

