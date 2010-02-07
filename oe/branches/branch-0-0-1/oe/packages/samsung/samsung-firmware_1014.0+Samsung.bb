require firmware.inc
require firmiszip.inc

ARM_INSTRUCTION_SET = "arm"

PR = "r0"

DL_URI = "http://downloadcenter.samsung.com/content/FM/201001/20100115170121859/2009_DTV_128M_firmware.exe"

# S = "${WORKDIR}/${MACHINE}"

# exe.img is squashfs, so editing rc.local is no problem
# P_OFFSET = ""
# P_LINE = ";/etc/telnetd_start.sh &"
require firmware-package.bbclass
