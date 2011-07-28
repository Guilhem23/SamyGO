#CROSS=sh4-linux-
#CC=$(CROSS)gcc
#LD=$(CROSS)ld
#STRIP=$(CROSS)strip
SH_CFLAGS=-m4 -Wall -Os -fno-reorder-functions -fno-unit-at-a-time
SOLIB_CFLAGS=$(SH_CFLAGS) -nostartfiles -nostdlib -shared
SOSTUB_CFLAGS=$(SOLIB_CFLAGS) -Lstub_stripped -lc_stub -lpthread_stub
OE_STAGE?=$(OE_BASE)/build/tmp/staging/sh4-linux

all: autobright.so libAutobright.so

autobright.so : autobright.c autobright.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(SOSTUB_CFLAGS) -o $@ $< -Wl,-Bstatic,-lusb,-Bdynamic

libAutobright.so: autobright-ui.c autobright.h
	$(CC) $(SH_CFLAGS) -o $@ $< `sdl-config-sh4-linux --cflags --libs` -lSDL_gfx -lSDL_ttf -shared

