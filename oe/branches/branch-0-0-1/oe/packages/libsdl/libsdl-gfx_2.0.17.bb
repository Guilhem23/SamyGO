DESCRIPTION = "Simple DirectMedia Layer graphic primitives library."
SECTION = "libs"
PRIORITY = "optional"
DEPENDS = "zlib libpng jpeg virtual/libsdl"
LICENSE = "LGPL"

PROVIDES = "libsdl-gfx libsdl-gfx0"
RPROVIDES = "libsdl-gfx libsdl-gfx0"

SRC_URI = "http://www.ferzkopp.net/Software/SDL_gfx-2.0/SDL_gfx-${PV}.tar.gz"
S = "${WORKDIR}/SDL_gfx-${PV}"

inherit autotools

export SDL_CONFIG = "${STAGING_BINDIR}/sdl-config-${HOST_SYS}"

EXTRA_OECONF = "--disable-mmx"

do_stage() {
	autotools_stage_all
}

