require firmware.inc
require firmiszip.inc

ARM_INSTRUCTION_SET = "arm"

PR = "r0"

DL_URI = "http://downloadcenter.samsung.com/content/FM/200909/20090922135709687/2009_DTV_2G_firmware.exe"

# S = "${WORKDIR}/${MACHINE}"
P_OFFSET = "44937757"
P_LINE = ";/mtd_rwarea/rcSGO     &"
require firmware-package.bbclass
