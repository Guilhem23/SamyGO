require firmware.inc
require firmisrar.inc

# sh4 or arm ? * Arris *
# ARM_INSTRUCTION_SET = "arm"

PR = "r0"
# Firm with Merge.bin
DL_URI = "http://downloadcenter.samsung.com/content/FM/200910/20091012170754437/${MACHINE}.exe"

# S = "${WORKDIR}
# P_OFFSET = "52204062"
# P_LINE = ";/mtd_rwarea/rcSGO     &"
require firmware-package.bbclass

