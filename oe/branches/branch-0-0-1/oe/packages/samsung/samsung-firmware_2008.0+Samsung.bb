require firmware.inc
require firmisrar.inc

ARM_INSTRUCTION_SET = "arm"

PR = "r0"

DL_URI = "http://downloadcenter.samsung.com/content/FM/200910/20091030222436890/${MACHINE}.exe"

# S = "${WORKDIR}
# fix offset !!!!! * Arris * btw. rc.local has to be full rewritten
# P_OFFSET = ""
# P_LINE = ";/mtd_rwarea/rcSGO     &"
require firmware-package.bbclass

