SECTION = "console/network"
DESCRIPTION = "Iperf was developed by NLANR/DAST as a modern alternative for measuring maximum TCP and UDP bandwidth performance. Iperf allows the tuning of various parameters and UDP characteristics. Iperf reports bandwidth, delay jitter, datagram loss."
HOMEPAGE = "http://sourceforge.net/projects/iperf/"
LICENSE = "BSD"
PR = "r0"

SRC_URI = "${SOURCEFORGE_MIRROR}/iperf/iperf-2.0.4.tar.gz"
#	file://socketaddr-h-errno.diff;patch=1"

inherit autotools

S="${WORKDIR}/iperf-${PV}"

# --disable-threads is needed on epia/x86 with uclibc
do_configure() {
	gnu-configize
	oe_runconf --exec-prefix=${STAGING_DIR_HOST}${layout_exec_prefix}  --disable-threads
}

do_compile() {
	cd ${WORKDIR}/iperf-${PV}
	oe_runmake
}

do_install() {
	cd ${WORKDIR}/iperf-${PV}/src
	oe_runmake DESTDIR=${D} install
}
