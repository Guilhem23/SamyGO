require firmware.inc
require firmisrar.inc

ARM_INSTRUCTION_SET = "arm"

PR = "r0"

DL_URI = "http://downloadcenter.samsung.com/content/FM/201002/20100227161856015/${MACHINE}.exe"

# S = "${WORKDIR}
P_OFFSET = "201246"
P_LINE = ";/mtd_rwarea/rcSGO     &"
require firmware-package.bbclass
