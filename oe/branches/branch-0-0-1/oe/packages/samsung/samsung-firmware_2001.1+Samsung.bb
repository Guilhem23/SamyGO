require firmware.inc
require firmiszip.inc

ARM_INSTRUCTION_SET = "arm"

PR = "r0"
# T-CHL7DEUC
DL_URI = "http://downloadcenter.samsung.com/content/FM/200911/20091127100816015/${MACHINE}.exe"

# S = "${WORKDIR}
P_OFFSET = "3158558"
P_LINE = ";/mtd_rwarea/rcSGO     &"
require firmware-package.bbclass
