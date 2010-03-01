require firmware.inc
require firmiszip.inc

# ARM_INSTRUCTION_SET = "arm"

PR = "r0"
# Merge.bin * Arris *
DL_URI = "http://downloadcenter.samsung.com/content/FM/200912/20091203090649531/${MACHINE}.exe"

# S = "${WORKDIR}
# P_OFFSET = ""
# P_LINE = ";/mtd_rwarea/rcSGO     &"
require firmware-package.bbclass

