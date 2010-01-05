DESCRIPTION = "Samsung-Patched SDL-Library for Chelsea Platform"
HOMEPAGE = "http://www.samsung.com/global/opensource/files"
SECTION = "libs"
LICENSE = "GPL/Samsung/Yahoo"
PV ="1.2.11"
PR = "r01"

PROVIDES = "virtual/libsdl virtual/libx11"

# oe_note "Needs sdl-config Fix!!"

SRC_URI = "http://www.samsung.com/global/opensource/files/32B650.zip \
		file://sdl-config \
"
INHIBIT_PACKAGE_STRIP = "1"

LEAD_SONAME = "libSDL.so"

inherit pckgconfig autotools

S = "${WORKDIR}"

do_unpack2() {
        unzip -o -q ${WORKDIR}/SDL-${PV}.zip -d ${WORKDIR}/
	cd ${WORKDIR}
	rm -rf "*.tgz" "*.zip" "SELP*" || true
 	# Branding
}

addtask unpack2 before do_patch after do_unpack

do_configure_samygo () {
	:
}

do_compile_samygo () {
	:
}

do_stage_samygo () {
	ls -l
	oe_libinstall -so -C Lib/CHELSEA libSDL ${STAGING_LIBDIR}/
	cp -a include ${STAGING_INCDIR}/SDL
	cp -a sdl-config ${STAGING_BINDIR}/
}

do_install_samygo () {
	install -d ${D}/lib
        install Lib/CHELSEA/libSDL.so ${D}/lib/	
}

do_configure () {
	cp Makefile.SDL.CHELSEA Makefile
	perl -pi -e "s/OP_Product\/Engine\/SDL\/SDL-1.2.11//" Makefile
	perl -pi -e "s/CROSS_COMPILE = .*/CROSS_COMPILE = ${TARGET_PREFIX}/" Makefile
	mkdir -p fake/AP_GPlayer/Porting/Inc
	mkdir -p fake/AP_Yahoo/SDL_HWAcceleration/Chelsea/Inc
	echo "void VideoClose(void);" > fake/AP_GPlayer/Porting/Inc/GPlayerPorting.h
	echo "/* Dummy */" > fake/AP_Yahoo/SDL_HWAcceleration/Chelsea/Inc/YahooSDLHWAcceleration.h
}

do_compile () {
	:
}

do_install () {
	:
}
