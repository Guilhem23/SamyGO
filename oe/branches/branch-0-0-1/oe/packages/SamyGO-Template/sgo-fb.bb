DESCRIPTION = "Half-resolution framebuffer (?)"
LICENSE = "GPL"
SECTION = "SamsungDTV/Commandline"
HOMEPAGE = "http://sourceforge.net/apps/phpbb/samygo"
MAINTAINER = "http://sourceforge.net/apps/phpbb/samygo/memberlist.php?mode=viewprofile&u=550"

PV = "0.1"
PR = "r01"

# COMPATIBLE_HOST = "arm.*-linux"

# DEPENDS = "openssl"
# RDEPENDS = "libsdl-gfx0"

FILES_${PN} = "/bin/*"

# inherit autotools pkgconfig

FILES = "half-res-fb-poc_020210.tar.gz"
SRC_URI = "http://sourceforge.net/apps/phpbb/samygo/download/${FILES} \
#		file://Makefile \
"
DL_URI = "http://sourceforge.net/apps/phpbb/samygo/download/file.php?id=96"
FETCHCOMMAND_wget = "wget ${DL_URI} -O ${DL_DIR}/${FILES}"

S = ${WORKDIR}/src
# EXTRA_OEMAKE = "EXTRA_CFLAGS=-I${STAGING_INCDIR}/SDL"
# EXTRA_CFLAGS += "-I${STAGING_INCDIR}/SDL"

do_compile () {
	${CC} -o gfx-test1 gfx-test1.c 
}

do_install () {
        install -d ${D}/bin
        install -m 0755 ${S}/gfx-test1 ${D}/bin/
}

