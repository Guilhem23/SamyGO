require firmware.inc
require firmiszip.inc

ARM_INSTRUCTION_SET = "arm"

PR = "r0"

# LNxxA75x Firmware (ARM)

DL_URI = "http://downloadcenter.samsung.com/content/FM/200810/20081021143429656/${MACHINE}.exe"

#S = "${WORKDIR}/${MACHINE}"

require firmware-package.bbclass
