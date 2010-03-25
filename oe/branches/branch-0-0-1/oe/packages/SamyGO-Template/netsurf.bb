DESCRIPTION = "Lightweight web browser capable of handling many of the web standards in use today."
DESCRIPTION += "port to samsungdtv:"
DESCRIPTION += "http://sourceforge.net/apps/phpbb/samygo"
DESCRIPTION += "by https://sourceforge.net/apps/phpbb/samygo/memberlist.php?mode=viewprofile&u=711"
HOMEPAGE = "http://www.netsurf-browser.org/"
SECTION = "x11/network"
LICENSE = "GPLv2"

PV = "2.1-SGO"
PR = "r01"

INHIBIT_PACKAGE_STRIP = "1"

PACKAGES = "${PN}-dbg ${PN} ${PN}-bin ${PN}-dev ${PN}-doc ${PN}-locale"

SRC_URI = "http://rrobek.de/download/netsurf-src.tgz \
		file://netsurf.png \
"

S = ${WORKDIR}/${PN}-pc/netsurf

#        libz.so.1 => not found
#        libssl.so.0.9.7 => not found
#        libcurl.so.4 => not found
#        libfreetype.so.6 => not found
#        libjpeg.so.62 => not found
#        libm.so.6 => /lib/libm.so.6 (0x4021b000)
#        libSDL.so => /mtd_cmmlib/GAME_LIB/libSDL.so (0x4028d000)
#        libdl.so.2 => /lib/libdl.so.2 (0x402d2000)
#        librt.so.1 => /lib/librt.so.1 (0x402dd000)
#        libpthread.so.0 => /lib/libpthread.so.0 (0x402ec000)
#        libidn.so.11 => not found
#        libcrypto.so.0.9.7 => not found
#        libgcc_s.so.1 => /lib/libgcc_s.so.1 (0x4030a000)
#        libc.so.6 => /lib/libc.so.6 (0x4031c000)
#        /lib/ld-linux.so.3 (0x2a000000)

DEPENDS = "virtual/libsdl virtual/libiconv libidn lemon-native re2c-native \
		openssl curl libxml2 libnsbmp hubbub libnsgif libmng lcms libsdl-ttf jpeg libpng"

EXTRA_OEMAKE = "TARGET=framebuffer HOST=SamsungDTV CURDIR=${S} DESTDIR=${D} PREFIX=${prefix}"

EXTRA_OEMAKE_append_sh4 += " WARNFLAGS='-Wno-error'"
EXTRA_OEMAKE_append_arm += " WARNFLAGS='-Wno-error -fgnu89-inline'"
EXTRA_OEMAKE_append_samygo += " NETSURF_FB_FRONTEND=sdl NETSURF_FB_RESPATH_sdl=./framebuffer/res/ \
				GCCSDK_INSTALL_ENV=${STAGING_LIBDIR}/../ EXETARGET=libNetSurf.so \
				NETSURF_STRIP_BINARY=no"

LDFLAGS = " -fPIC -shared"

do_configure() {
	perl -pi -e "s/-liconv/-lpng12 -ljpeg -lfreetype -lxml2/g" Makefile
	perl -pi -e "s/(^NETSURF_USE_LIBICONV_PLUG)(.*)/NETSURF_USE_SVG \2/g" Makefile.config
	echo "override NETSURF_USE_SVG := NO" > Makefile.config
	echo "override NETSURF_FB_FONTLIB := freetype" >> Makefile.config
}

do_compile () {
	oe_runmake 
}

do_install_samygo() {
	install -d ${D}/framebuffer/res/
        oe_runmake install-framebuffer

	# Original Fonts? * Arris *
	cp -va framebuffer/res/*.ttf ${D}/framebuffer/res/

	mkdir -p ${D}/mtd_tlib/NetSurf/
        install -m 0755 libNetSurf.so ${D}/mtd_tlib/NetSurf/
        install -m 0644 ${WORKDIR}/netsurf.png ${D}/mtd_tlib/NetSurf/NetSurf.png
	mv ${D}/framebuffer ${D}/mtd_tlib/NetSurf/

        # Create .dat file
        for i in Name Category Size Vendor Release_Date Description ; do
                echo "[$i]" >> ${D}/mtd_tlib/NetSurf/NetSurf.dat
                        for j in 1 2 3 4 ; do
                                if [ "$i" == "Release_Date" ] ; then
                                        echo "$(date "+%Y/%m/%d" | xxd -i | tr a 0 | tr -d " ")" >> ${D}/mtd_tlib/NetSurf/NetSurf.dat
                                elif [ "$i" == "Size" ] ; then
                                        echo "$(printf "01 MB\0" | xxd -i | tr -d " ")" >> ${D}/mtd_tlib/NetSurf/NetSurf.dat
                                else
                                        echo "$(printf "NetSurf\0" | xxd -i | tr -d " ")" >> ${D}/mtd_tlib/NetSurf/NetSurf.dat
                                fi
                        done
        done
        echo '[JoyStick_Support]
0' >> ${D}/mtd_tlib/NetSurf/NetSurf.dat
        echo '[icon_path]
NetSurf.png


// End of Generated Text Resource Source File' >> ${D}/mtd_tlib/NetSurf/NetSurf.dat

# fixup .dat file
perl -pi -e "s/^\s+//g" ${D}/mtd_tlib/NetSurf/NetSurf.dat
perl -pi -e "s/0x/0x00/g" ${D}/mtd_tlib/NetSurf/NetSurf.dat
perl -pi -e "s/0x0000\n/0x0000,\n/g" ${D}/mtd_tlib/NetSurf/NetSurf.dat
perl -pi -e "s/\n/\x0d\x0a/g" ${D}/mtd_tlib/NetSurf/NetSurf.dat

      
  # Create clmeta.dat file
echo '<?xml version="1.0" encoding="utf-8"?>
<contentlibrary>
  <contentpack id="NetSurf">
    <category>Game</category>
    <title language_id="German">NetSurf Browser</title>
    <startpoint language_id="German">./libNetSurf.so</startpoint>
    <title language_id="English">NetSurf Browser</title>
    <startpoint language_id="English">./libNetSurf.so</startpoint>
    <thumbnailpath>./NetSurf.png</thumbnailpath>
    <totalsize>1</totalsize>
  </contentpack>
</contentlibrary>' > ${D}/mtd_tlib/NetSurf/clmeta.dat

}

FILES_${PN}-dbg += "/mtd_tlib/NetSurf/.debug"
FILES_${PN} += "/mtd_tlib"
