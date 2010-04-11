require pixman.inc
PR = "${INC_PR}.0"

NEON = " --disable-arm-neon "

EXTRA_OECONF = "${NEON} --disable-gtk"
