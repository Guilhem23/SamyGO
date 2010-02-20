require firmware.inc
require firmiszip.inc

ARM_INSTRUCTION_SET = "arm"

PR = "r0"
# Untested * Arris *
DL_URI = "http://downloadcenter.samsung.com/content/FM/200809/20080910134341046_L530_firmware_1007.0.exe"

# S = "${WORKDIR}
P_OFFSET = "11571255"
P_LINE = "/mtd_rwarea/rcSGO  &"
require firmware-package.bbclass

