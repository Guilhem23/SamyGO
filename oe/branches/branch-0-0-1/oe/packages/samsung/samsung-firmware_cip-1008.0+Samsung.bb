require firmware.inc
require firmisrar.inc

ARM_INSTRUCTION_SET = "arm"

PR = "r0"

DL_URI_T-CHL5CIPDSEUC = "http://downloadcenter.samsung.com/content/FM/200912/20091228110743609/${MACHINE}.exe"
DL_URI = "http://downloadcenter.samsung.com/content/FM/200912/20091228111122781/${MACHINE}.exe"

# S = "${WORKDIR}


P_OFFSET = "16372254"
P_LINE = ";/mtd_rwarea/rcSGO     &"

P_OFFSET_T-CHL5CIPDSEUC = ""
P_LINE_T-CHL5CIPDSEUC = ""

do_patch_prepend_T-CHL5CIPDSEUC() {
	oewarn "on T-CHL5CIPDSEUC dont patch rc.local !!!"
}

require firmware-package.bbclass

