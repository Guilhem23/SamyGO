PR = "r15"

require gcc-${PV}.inc
require gcc-configure-target.inc
require gcc-package-target.inc

ARCH_FLAGS_FOR_TARGET += "-isystem${STAGING_INCDIR}"

SRC_URI[md5sum] = "232c2397d511253a8d6dcf66fa8d9bc2"
SRC_URI[sha256sum] = "78cecf54357d014450355c35af21d2ad330c782a89f56fead6a65b238dbff0f6"
