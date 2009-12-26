DESCRIPTION = "xor files"
SECTION = "console/utils"
PV = "2.1"
PR = "2"

SRC_URI = "http://crypt-xor.sourceforge.net/stable/crypt-xor_${PV}-${PR}.tar.gz"

inherit native

do_compile() {
	cp ${WORKDIR}/crypt-xor-${PV}/crypt-XOR.c .
	${CXX} -I. -o crypt-xor crypt-XOR.c
}

do_stage() {
	install -m 0755 crypt-xor ${STAGING_BINDIR}/
}
