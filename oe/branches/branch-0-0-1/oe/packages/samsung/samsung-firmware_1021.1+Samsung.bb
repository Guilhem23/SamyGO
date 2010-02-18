require firmware.inc
require firmiszip.inc

ARM_INSTRUCTION_SET = "arm"

PR = "r0"

# PN50B530
# unknown * Arris *
DL_URI = "http://downloadcenter.samsung.com/content/FM/201002/20100205143934296/2009_DTV_S4_firmware.exe"

# S = "${WORKDIR}/${MACHINE}"
# P_OFFSET = "44937757"
# P_LINE = ";/mtd_rwarea/rcSGO     &"
require firmware-package.bbclass
