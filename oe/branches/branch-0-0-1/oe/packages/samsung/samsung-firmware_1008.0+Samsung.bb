require firmware.inc
require firmiszip.inc

ARM_INSTRUCTION_SET = "arm"

PR = "r0"

DL_URI = "http://downloadcenter.samsung.com/content/FM/200908/20090806125546515/${MACHINE}.exe"
DL_URI_T-PRLAUS = "http://downloadcenter.samsung.com/content/FM/200809/20080910134341046_L530_firmware_1008.0.exe"

# S = "${WORKDIR}
P_OFFSET = "3158558"
P_LINE = ";/mtd_rwarea/rcSGO     &"

P_OFFSET_T-PRLAUS = "11661367"
P_LINE_T-PRLAUS = "/mtd_rwarea/rcSGO  &"

require firmware-package.bbclass

