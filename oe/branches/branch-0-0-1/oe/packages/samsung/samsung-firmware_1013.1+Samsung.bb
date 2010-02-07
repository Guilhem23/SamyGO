require firmware.inc
require firmisrar.inc

SH4_INSTRUCTION_SET = "sh"

PR = "r0"

# LExxA75x Firmware (SH)

DL_URI = "http://downloadcenter.samsung.com/content/FM/200905/20090512010728671/T-RBYDEU.exe"

#S = "${WORKDIR}/${MACHINE}"

require firmware-package.bbclass
