DESCRIPTION = "SamyGO Extensions ${PN}"
LICENSE = "GPL"
SECTION = "Gallery/Games"
MAINTAINER = "Ser Lev Arris <arris@ZsoltTech.Com>"
HOMEPAGE = "http://samygo.sourceforge.net"

PR = "r01"

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
 	${CC} ${TARGET_CFLAGS} -O2 -Wall `sdl-config-${HOST_SYS} --cflags --libs` -fPIC -shared -o SamyGOE.so SamyGOE.c 
}

do_install () {
        mkdir -p ${D}/mtd_tlib/SamyGO/
        install -m 0755 SamyGOE.so ${D}/mtd_tlib/SamyGO/
        install -m 0644 SamyGO.png ${D}/mtd_tlib/SamyGO/
	
	# Create clmeta.dat file
echo '<?xml version="1.0" encoding="utf-8"?>
<contentlibrary>
  <contentpack id="SamyGO">
    <category>Game</category>
    <title language_id="German">SamyGO Erweiterungen</title>
    <startpoint language_id="German">./SamyGOE.so</startpoint>
    <title language_id="English">SamyGO Extensions</title>
    <startpoint language_id="English">./SamyGOE.so</startpoint>
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

