PN_ORIG = libelf
require ${PN_ORIG}_${PV}.bb
S = "${WORKDIR}/${PN_ORIG}-${PV}"
inherit native

