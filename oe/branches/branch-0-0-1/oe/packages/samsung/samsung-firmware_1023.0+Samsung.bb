require firmware.inc
require firmisrar.inc

# arm or sh4 ? * Arris *
# SH4_INSTRUCTION_SET = "sh"

PR = "r0"

# LE46A656A Firmware (SH)
# MT8226/T-AMBPEUMD-1023.0.bin
DL_URI = "http://downloadcenter.samsung.com/content/FM/200905/20090512003238437/${MACHINE}.exe"

#S = "${WORKDIR}/${MACHINE}"
# P_OFFSET = "44937757"
# P_LINE = ";/mtd_rwarea/rcSGO     &"
require firmware-package.bbclass
