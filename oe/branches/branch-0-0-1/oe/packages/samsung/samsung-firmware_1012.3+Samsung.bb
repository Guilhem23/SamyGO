require firmware.inc
require firmiszip.inc

ARM_INSTRUCTION_SET = "arm"

PR = "r0"

DL_URI = "http://downloadcenter.samsung.com/content/FM/200909/20090922133700859/2009_DTV_1G_firmware.exe"

# S = "${WORKDIR}/${MACHINE}"
P_OFFSET = "3985949"
P_LINE = ";/mtd_rwarea/rcSGO     &"
require firmware-package.bbclass
