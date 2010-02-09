require firmware.inc
require firmisrar.inc

SH4_INSTRUCTION_SET = "sh"

PR = "r0"

# LExxA676 Firmware (SH)

DL_URI = "http://downloadcenter.samsung.com/content/FM/200906/20090629182553812/T-AMBDFR.exe"

#S = "${WORKDIR}/${MACHINE}"
# P_OFFSET = "44937757"
# P_LINE = ";/mtd_rwarea/rcSGO     &"
require firmware-package.bbclass
