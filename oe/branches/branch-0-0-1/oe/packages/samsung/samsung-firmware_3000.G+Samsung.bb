require firmware.inc
require firmisrar.inc

ARM_INSTRUCTION_SET = "arm"

PR = "r0"

DL_URI = "http://downloadcenter.samsung.com/content/FM/200911/20091124161213125/${MACHINE}.exe"

# S = "${WORKDIR}
P_OFFSET = "3969566"
P_LINE = ";/mtd_rwarea/rcSGO     &"
require firmware-package.bbclass
