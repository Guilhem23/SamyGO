require firmware.inc
require firmiszip.inc

ARM_INSTRUCTION_SET = "arm"

PR = "r0"

DL_URI = "http://downloadcenter.samsung.com/content/FM/200909/20090922133700859/2009_DTV_1G_firmware.exe"

# S = "${WORKDIR}/${MACHINE}"
P_OFFSET = "37757"  # not correct!!!
P_LINE = ";/etc/telnetd_start.sh &"
require firmware-package.bbclass