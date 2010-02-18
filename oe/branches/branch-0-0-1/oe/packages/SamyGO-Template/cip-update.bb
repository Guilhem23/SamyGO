DESCRIPTION = "Encryption/Decryption of Update files."
LICENSE = "GPL"
SECTION = "Gallery/Others"
HOMEPAGE = "http://sourceforge.net/apps/phpbb/samygo"
MAINTAINER = "https://sourceforge.net/apps/phpbb/samygo/memberlist.php?mode=viewprofile&u=365"

PV = "0.1"
PR = "r01"

# COMPATIBLE_HOST = "arm.*-linux"

# DEPENDS = "openssl"
# RDEPENDS = "libsdl-gfx0"

FILES_${PN} = "/lib/*"

# inherit autotools pkgconfig

FILES = "cip-update.zip"
SRC_URI = "http://sourceforge.net/apps/phpbb/samygo/download/${FILES} \
#		file://Makefile \
"
DL_URI = "http://sourceforge.net/apps/phpbb/samygo/download/file.php?id=61"
FETCHCOMMAND_wget = "wget ${DL_URI} -O ${DL_DIR}/${FILES}"

# S = ${WORKDIR}/${PN}
# EXTRA_OEMAKE = "EXTRA_CFLAGS=-I${STAGING_INCDIR}/SDL"
# EXTRA_CFLAGS += "-I${STAGING_INCDIR}/SDL"

# do_compile () {
#	cp -vf ${WORKDIR}/Makefile ${S}/Makefile
#	oe_runmake CROSS_COMPILE=${TARGET_PREFIX} TARGET=${PN}.so OBJ_DIR=${S}
#}

do_install () {
        install -d ${D}/lib
        install -m 0755 ${S}/${PN}.so ${D}/lib/
}

