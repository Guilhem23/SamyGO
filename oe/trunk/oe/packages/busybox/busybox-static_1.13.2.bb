require busybox_${PV}.bb

FILESDIR = "${@os.path.dirname(bb.data.getVar('FILE',d,1))}/busybox-${PV}"

S = "${WORKDIR}/busybox-${PV}"

RRECOMMENDS = ""

export CFLAGS:="${CFLAGS} -static"
