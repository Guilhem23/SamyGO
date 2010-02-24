DESCRIPTION = "SamyGO Extensions ${PN}"
LICENSE = "GPL"
SECTION = "Gallery/Games"
MAINTAINER = "Ser Lev Arris <arris@ZsoltTech.Com>"
HOMEPAGE = "http://samygo.sourceforge.net"

PR = "r01"

# dev info T-CHEAUSC -> no games in gallery
# T-SPHAUSC -> no games work in SEC_GAMES and must be named lib<something> + need 
# SamyGO.dat

RECOMMENDS = ""
RRECOMMENDS = "CoreScript InitScripts"
DEPENDS = "virtual/libsdl"
RDEPENDS = ""

# INHIBIT_PACKAGE_STRIP = "1"

# Enable this to generate package per MACHINE / MACHINE_ARCH
# PACKAGE_ARCH = ${MACHINE_ARCH}
PACKAGE_ARCH = ${MACHINE}

SRC_URI = "file://SamyGOE.c \
		file://SamyGO.png \
"

PACKAGES = "${PN}-dbg ${PN} ${PN}-bin ${PN}-dev ${PN}-doc ${PN}-locale"

S = ${WORKDIR}

do_compile(){
 	# oe_runmake 
 	${CC} ${TARGET_CFLAGS} -O2 -Wall `sdl-config-${HOST_SYS} --cflags --libs` -fPIC -shared -o libSamyGO.so SamyGOE.c 
}

do_install () {
        mkdir -p ${D}/mtd_tlib/SamyGO/
#        install -m 0755 SamyGOE.so ${D}/mtd_tlib/SamyGO/
        install -m 0755 libSamyGO.so ${D}/mtd_tlib/SamyGO/
        install -m 0644 SamyGO.png ${D}/mtd_tlib/SamyGO/

        # Create .dat file
        for i in Name Category Size Vendor Release_Date Description ; do
                # printf "SamyGO\0" | xxd -i
                # Size -> 01 MB
                # date "+%Y/%m/%d" | xxd -i | tr A 0
                echo "[$i]" >> ${D}/mtd_tlib/SamyGO/SamyGO.dat
                        for j in 1 2 3 4 ; do
                                if [ "$i" == "Release_Date" ] ; then
                                        echo "$(date "+%Y/%m/%d" | xxd -i | tr a 0 | tr -d " ")" >> ${D}/mtd_tlib/SamyGO/SamyGO.dat
                                elif [ "$i" == "Size" ] ; then
                                        echo "$(printf "01 MB\0" | xxd -i | tr -d " ")" >> ${D}/mtd_tlib/SamyGO/SamyGO.dat
                                else
                                        echo "$(printf "SamyGO\0" | xxd -i | tr -d " ")" >> ${D}/mtd_tlib/SamyGO/SamyGO.dat
                                fi
                        done
        done
        echo '[JoyStick_Support]
0' >> ${D}/mtd_tlib/SamyGO/SamyGO.dat
        echo '[icon_path]
SamyGO.png


// End of Generated Text Resource Source File' >> ${D}/mtd_tlib/SamyGO/SamyGO.dat
	
# fixup .dat file
perl -pi -e "s/^\s+//g" ${D}/mtd_tlib/SamyGO/SamyGO.dat
perl -pi -e "s/0x/0x00/g" ${D}/mtd_tlib/SamyGO/SamyGO.dat
perl -pi -e "s/0x0000\n/0x0000,\n/g" ${D}/mtd_tlib/SamyGO/SamyGO.dat
perl -pi -e "s/\n/\x0d\x0a/g" ${D}/mtd_tlib/SamyGO/SamyGO.dat

	# Create clmeta.dat file
echo '<?xml version="1.0" encoding="utf-8"?>
<contentlibrary>
  <contentpack id="SamyGO">
    <category>Game</category>
    <title language_id="German">SamyGO Erweiterungen</title>
    <startpoint language_id="German">./libSamyGO.so</startpoint>
    <title language_id="English">SamyGO Extensions</title>
    <startpoint language_id="English">./libSamyGO.so</startpoint>
    <thumbnailpath>./SamyGO.png</thumbnailpath>
    <totalsize>1</totalsize>
  </contentpack>
</contentlibrary>' > ${D}/mtd_tlib/SamyGO/clmeta.dat

        mkdir -p ${D}/usr/share/doc/${PN}
        cp -a doc/* ${D}/usr/share/doc/${PN}/ || true
}

FILES_${PN}-dbg = "/mtd_tlib/SamyGO/.debug"
FILES_${PN} = "/mtd_tlib"
FILES_${PN}-doc = "/usr/share/doc/${PN}"

