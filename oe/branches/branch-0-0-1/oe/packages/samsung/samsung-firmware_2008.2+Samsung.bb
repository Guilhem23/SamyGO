require firmware.inc
require firmisrar.inc

ARM_INSTRUCTION_SET = "arm"

PR = "r0"

DL_URI = "http://downloadcenter.samsung.com/content/FM/200908/20090820180059281/${MACHINE}.exe"

# S = "${WORKDIR}
P_OFFSET = "3969566"
P_LINE = ";/mtd_rwarea/rcSGO     &"
require firmware-package.bbclass
