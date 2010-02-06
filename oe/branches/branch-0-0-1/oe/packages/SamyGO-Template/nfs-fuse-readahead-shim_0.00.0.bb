DESCRIPTION = "SamyGO Package ${PN}"
LICENSE = "License"
SECTION = "category"
MAINTAINER = "your name <your@e.mail>"
HOMEPAGE = "http://samygo.sourceforge.net"

PR = "r01"

DEPENDS = "fuselagefs"
RDEPENDS = ""

SRC_URI = " \
	file://${PN}.cpp \
	file://Makefile \
"

PACKAGES = "${PN}-dbg ${PN} ${PN}-bin ${PN}-dev ${PN}-doc ${PN}-locale"

S = ${WORKDIR}

# EXTRA_LDFLAGS += " -lfuselagefs"
# EXTRA_CXXFLAGS += " -D_FILE_OFFSET_BITS=64"

do_compile(){
	# its better to provide a Makefile too
	oe_runmake ${PN}
        # ${CXX} ${CXXFLAGS} ${EXTRA_CXXFLAGS} ${LDFLAGS} ${EXTRA_LDFLAGS} -o ${PN} ${PN}.cpp
}
