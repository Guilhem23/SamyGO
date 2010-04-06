DESCRIPTION = "SDL VNC client"
LICENSE = "GPL"
SECTION = "base"
PRIORITY = "optional"
DEPENDS = "libsdl-cl"

SRC_URI = "file://SDL_vnc.c file://SDL_vnc.h file://d3des.c file://d3des.h file://main.c"

S = "${WORKDIR}/vnc-client-cl-${PV}"

PR = "r0"

do_configure() {
	install -m 0644 ${WORKDIR}/*.c ${S}/
	install -m 0644 ${WORKDIR}/*.h ${S}/
}

do_compile() {
	${CC} ${S}/SDL_vnc.c ${S}/d3des.c ${S}/main.c -shared `${STAGING_BINDIR}/sdl-config --libs` \
		`${STAGING_BINDIR}/sdl-config --cflags` ${CFLAGS} ${LDFLAGS} -o ${S}/libvnc-client.so
}

FILES_${PN} = "/vnc-client"

do_install() {
	install -d ${D}/vnc-client
	install -m 0644 ${S}/libvnc-client.so ${D}/vnc-client/libvnc-client.so
}

