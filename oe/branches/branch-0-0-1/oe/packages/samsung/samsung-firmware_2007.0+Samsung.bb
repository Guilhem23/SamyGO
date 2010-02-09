require firmware.inc
require firmiszip.inc

ARM_INSTRUCTION_SET = "arm"

PR = "r0"

# LN46A650 Firmware (ARM)
# squashfs ?
DL_URI = "http://downloadcenter.samsung.com/content/FM/200810/20081017150200187/${MACHINE}.exe"

#S = "${WORKDIR}/${MACHINE}"
# P_OFFSET = "44937757"
# P_LINE = ";/mtd_rwarea/rcSGO     &"
require firmware-package.bbclass
