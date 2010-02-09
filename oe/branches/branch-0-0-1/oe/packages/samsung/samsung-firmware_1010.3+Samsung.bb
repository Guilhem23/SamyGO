require firmware.inc
require firmisrar.inc

# ARM_INSTRUCTION_SET = "arm"

PR = "r0"
# Firm with Merge.bin ?
DL_URI = "http://fixmee/content/FM/200910/20091012170754437/T-CRLPEUHC.exe"

# S = "${WORKDIR}
# P_OFFSET = "52204062"
# P_LINE = ";/mtd_rwarea/rcSGO     &"
require firmware-package.bbclass

