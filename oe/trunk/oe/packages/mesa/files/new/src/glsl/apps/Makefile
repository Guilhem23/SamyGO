# src/glsl/apps/Makefile

TOP = ../../..

include $(TOP)/configs/current

LIBS = \
	$(TOP)/src/glsl/pp/libglslpp_host.a \
	$(TOP)/src/glsl/cl/libglslcl_host.a

SOURCES = \
	compile.c \
	process.c \
	purify.c \
	tokenise.c \
	version.c

APPS = $(SOURCES:%.c=%)

INCLUDES = -I.


##### RULES #####

.SUFFIXES:
.SUFFIXES: .c

.c:
	$(BUILD_CC) $(INCLUDES) $< $(LIBS) -o $@

.c.o:
	$(BUILD_CC) -c $(INCLUDES) $(DEFINES) $< -o $@


##### TARGETS #####

default: $(APPS)

install:

clean:
	-rm -f $(APPS)
	-rm -f *.o
