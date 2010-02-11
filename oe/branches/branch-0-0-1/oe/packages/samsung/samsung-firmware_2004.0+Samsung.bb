require firmware.inc
require firmisrar.inc

# sh4 or arm ? * Arris *
# ARM_INSTRUCTION_SET = "arm"

# PROVIDES = "virtual/kernel virtual/libc"
PR = "r0"
# Firm with Merge.bin
DL_URI = "http://downloadcenter.samsung.com/content/FM/200910/20091012170754437/${MACHINE}.exe"
DL_URI_T-CRLPEUWC = "http://downloadcenter.samsung.com/content/FM/200912/20091228105941859/${MACHINE}.exe"

# S = "${WORKDIR}
# P_OFFSET = "52204062"
# P_LINE = ";/mtd_rwarea/rcSGO     &"
require firmware-package.bbclass

