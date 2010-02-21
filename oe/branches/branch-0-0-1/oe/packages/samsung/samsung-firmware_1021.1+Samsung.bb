require firmware.inc
require firmiszip.inc

ARM_INSTRUCTION_SET = "arm"

PR = "r0"

# PN50B530 PN58B550
# unknown * Arris *
# Archive: /usr/src/Tuxbox/Archive/T-STL5PAUSFC-1021.1+Samsung.exe
# inflating: T-STL5PAUSFC/system_BL_FHD.m51
# inflating: T-STL5PAUSFC/system_L_FHD.m51
# inflating: T-STL5PAUSFC/system_SB.m51
# inflating: T-STL5PAUSFC/T_USFH.BIN
#extracting: T-STL5PAUSFC/T_USFH.txt
# inflating: T-STL3FAUSHC/system_BL_HD.m51
# inflating: T-STL3FAUSHC/system_L_HD.m51
# inflating: T-STL3FAUSHC/system_SB.m51
# inflating: T-STL3FAUSHC/T_USHD.BIN
# extracting: T-STL3FAUSHC/T_USHD.txt

DL_URI = "http://downloadcenter.samsung.com/content/FM/201002/20100205143934296/2009_DTV_S4_firmware.exe"

# S = "${WORKDIR}/${MACHINE}"
P_OFFSET = ""
# P_LINE = ";/mtd_rwarea/rcSGO &"
require firmware-package.bbclass
