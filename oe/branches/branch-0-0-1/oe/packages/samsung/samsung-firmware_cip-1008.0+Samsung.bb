require firmware.inc
require firmisrar.inc

ARM_INSTRUCTION_SET = "arm"

PR = "r0"

DL_URI = "http://downloadcenter.samsung.com/content/FM/200912/20091228111122781/${MACHINE}.exe"

# S = "${WORKDIR}
P_OFFSET = "16372254"
P_LINE = ";/mtd_rwarea/rcSGO     &"
require firmware-package.bbclass

