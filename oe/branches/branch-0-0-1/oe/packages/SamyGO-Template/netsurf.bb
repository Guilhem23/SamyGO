DESCRIPTION = "Lightweight web browser capable of handling many of the web standards in use today."
HOMEPAGE = "http://www.netsurf-browser.org/"
SECTION = "x11/network"
LICENSE = "GPLv2"

PV = "2.1-SGO"
PR = "r01"

PACKAGES = "${PN}-dbg ${PN} ${PN}-bin ${PN}-dev ${PN}-doc ${PN}-locale"

SRC_URI = "http://rrobek.de/download/netsurf-src.tgz \
		file://netsurf.png \
"

S = ${WORKDIR}/${PN}-pc/netsurf

DEPENDS = "virtual/libsdl libiconv libidn lemon-native re2c-native \
		openssl curl libxml2 libnsbmp hubbub libnsgif lcms libsdl-ttf"

EXTRA_OEMAKE = "TARGET=framebuffer HOST=SamsungDTV CURDIR=${S} DESTDIR=${D} PREFIX=${prefix} WARNFLAGS='-Wno-error -fgnu89-inline'"
EXTRA_OEMAKE_append = " GCCSDK_INSTALL_ENV=${STAGING_LIBDIR}/../ "

do_configure() {
	make clean
}

do_compile () {
	oe_runmake 
}

do_install() {
# TODO: Install fontfiles too
	install -d ${D}/framebuffer/res/
        oe_runmake install-framebuffer

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
