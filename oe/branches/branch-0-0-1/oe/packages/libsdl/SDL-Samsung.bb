DESCRIPTION = "Samsung-Patched SDL-Library for Chelsea Platform"
HOMEPAGE = "http://www.samsung.com/global/opensource/files"
SECTION = "libs"
LICENSE = "GPL/Samsung/Yahoo"
PV ="1.2.11"
PR = "r03"

PROVIDES = "virtual/libsdl virtual/libx11"

SRC_URI = "http://www.samsung.com/global/opensource/files/32B650.zip \
		file://sdl-config \
		file://sdl.m4 \
"
SRC_URI_sh4 = "http://www.samsung.com/global/opensource/files/SDL-1.2.11.zip \
		file://sdl-config \
                file://sdl.m4 \
"

INHIBIT_PACKAGE_STRIP = "1"

LEAD_SONAME = "libSDL.so"

inherit pckgconfig autotools

S = "${WORKDIR}"

do_unpack2() {
	if [ ${TARGET_ARCH} == "arm" ] ; then
	unzip -o -q ${WORKDIR}/SDL-${PV}.zip -d ${WORKDIR}/
        cd ${WORKDIR}
        rm -rf "*.tgz" "*.zip" "SELP*" || true
        # Branding
	fi
}

addtask unpack2 before do_patch after do_unpack

do_stage_arm () {
	oe_libinstall -so -C Lib/CHELSEA libSDL ${STAGING_LIBDIR}/
	mkdir -p ${STAGING_INCDIR}/SDL
	cp -a include/*.h ${STAGING_INCDIR}/SDL
        install src/video/SDL_sysvideo.h ${STAGING_INCDIR}/SDL/
        install src/video/SDL_glfuncs.h ${STAGING_INCDIR}/SDL/

echo "#!/bin/sh

prefix=${STAGING_LIBDIR}/..
exec_prefix=${STAGING_LIBDIR}/..
exec_prefix_set=no
" >> sdl-config.tmp
	cat sdl-config >> sdl-config.tmp
	mv sdl-config.tmp sdl-config
	install sdl.m4 ${STAGING_DATADIR}/aclocal/ 	
	install -m 755 sdl-config ${STAGING_BINDIR}/sdl-config-${HOST_SYS}
}

do_install_arm () {
	install -d ${D}/lib
        install Lib/CHELSEA/libSDL.so ${D}/lib/	
}

do_configure_sh4 () {
	cp -fv Makefile.STI Makefile
	perl -pi -e "s/OP_Product\/Engine\/SDL\/SDL-1.2.11//" Makefile
	perl -pi -e "s/CROSS_COMPILE = .*/CROSS_COMPILE = ${TARGET_PREFIX}/" Makefile
	mkdir -p GEngine/Porting/Inc
	mkdir -p fake/AP_Yahoo/SDL_HWAcceleration/Chelsea/Inc
	echo "void VideoClose(void);" > GEngine/Porting/Inc/GPlayerPorting.h
	echo "/* Dummy */" > fake/AP_Yahoo/SDL_HWAcceleration/Chelsea/Inc/YahooSDLHWAcceleration.h
	echo "/* Dummy */" > YahooSDLHWAcceleration.h
	mkdir lib_STI
	rm -f src/*.o
	rm -f src/*/*.o
	rm -f src/*/*/*.o
}

do_compile_sh4 () {
        oe_runmake COMP_DIR=${S} OPENSOURCE_DIR=${S}
}

do_install_sh4(){
        install -d ${D}/lib
        install lib_STI/libSDL.so ${D}/lib/
}

do_stage_sh4 () {
        ls -l
        oe_libinstall -so -C lib_STI libSDL ${STAGING_LIBDIR}/
        mkdir -p ${STAGING_INCDIR}/SDL
        cp -a include/*.h ${STAGING_INCDIR}/SDL
	install src/video/SDL_sysvideo.h ${STAGING_INCDIR}/SDL/
        install src/video/SDL_glfuncs.h ${STAGING_INCDIR}/SDL/

echo "#!/bin/sh

prefix=${STAGING_LIBDIR}/..
exec_prefix=${STAGING_LIBDIR}/..
exec_prefix_set=no
" >> sdl-config.tmp
        cat sdl-config >> sdl-config.tmp
        mv sdl-config.tmp sdl-config
        install sdl.m4 ${STAGING_DATADIR}/aclocal/
        # install -m 755 sdl-config ${STAGING_BINDIR}/
        install -m 755 sdl-config ${STAGING_BINDIR}/sdl-config-${HOST_SYS}
}

do_configure () {
	:
}

do_compile () {
	:
}

do_install () {
	:
}

do_stage () {
	:
}
