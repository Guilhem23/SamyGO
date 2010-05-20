DESCRIPTION = "SDL VNC client"
LICENSE = "GPL"
SECTION = "base"
PRIORITY = "optional"
DEPENDS = "libsdl-cl"

SRC_URI = "file://SDL_vnc.c file://SDL_vnc.h file://d3des.c file://d3des.h file://main.c \
	file://samygo-cl.png file://clmeta.dat file://AUTHORS file://COPYING file://ChangeLog \
	file://LICENSE file://README file://README.SamyGO "

S = "${WORKDIR}/vnc-client-cl-${PV}"

PR = "r0"

do_configure() {
	install -m 0644 ${WORKDIR}/*.c ${S}/
	install -m 0644 ${WORKDIR}/*.h ${S}/
}

do_compile() {
	${CC} ${S}/SDL_vnc.c ${S}/d3des.c ${S}/main.c -shared `sdl-config --libs` \
		`sdl-config --cflags` ${CFLAGS} ${LDFLAGS} -o ${S}/libvnc-client.so
}

FILES_${PN} = "/vnc-client"

do_install() {
	install -d ${D}/vnc-client
	install -m 0644 ${S}/clmeta.dat ${D}/vnc-client/clmeta.dat
	install -m 0644 ${S}/libvnc-client.so ${D}/vnc-client/libvnc-client.so
	install -m 0644 ${S}/samygo-cl.png ${D}/vnc-client/libvnc-client.png
	install -m 0644 ${S}/AUTHORS ${D}/vnc-client/AUTHORS
	install -m 0644 ${S}/COPYING ${D}/vnc-client/COPYING
	install -m 0644 ${S}/ChangeLog ${D}/vnc-client/ChangeLog
	install -m 0644 ${S}/LICENSE ${D}/vnc-client/LICENSE
	install -m 0644 ${S}/README ${D}/vnc-client/README
	install -m 0644 ${S}/README.SamyGO ${D}/vnc-client/README.SamyGO
}

