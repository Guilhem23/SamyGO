DESCRIPTION = "xor files"
SECTION = "console/utils"
PV = "2.1"
PR = "2"
S = "${WORKDIR}/crypt-xor-${PV}"

SRC_URI = "http://crypt-xor.sourceforge.net/stable/crypt-xor_${PV}-${PR}.tar.gz \
	file://fix_compile.patch;patch=1 \
"

inherit native

do_compile() {
	${CXX} -I. -o crypt-xor crypt-XOR.c
}

do_stage() {
	install -m 0755 crypt-xor ${STAGING_BINDIR}/
}