require firmware.inc
require firmiszip.inc

SH4_INSTRUCTION_SET = "sh"

PR = "r0"

# LNxxA75x Firmware (SH)

DL_URI = "http://downloadcenter.samsung.com/content/FM/200810/20081021143429656/${MACHINE}.exe"

#S = "${WORKDIR}/${MACHINE}"

require firmware-package.bbclass
