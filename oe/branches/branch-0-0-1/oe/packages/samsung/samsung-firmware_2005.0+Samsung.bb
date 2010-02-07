require firmware.inc
require firmisrar.inc

ARM_INSTRUCTION_SET = "arm"

oefatal "not clear what firmware or type"

PR = "r0"
# T-CHL7DEUC ? * Arris *
# T-CHEAEAC or this one ?
DL_URI = "http://downloadcenter.samsung.com/content/FM/200911/20091113134705734/T-CHL7DEUC.exe"
# DL_URI = "http://downloadcenter.samsung.com/content/FM/200910/20091030222802906/${MACHINE}.exe"

# S = "${WORKDIR}
P_OFFSET = "3969566"
P_LINE = ";/mtd_rwarea/rcSGO     &"
require firmware-package.bbclass
