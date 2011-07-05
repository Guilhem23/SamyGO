#CROSS=sh4-linux-
#CC=$(CROSS)gcc
#LD=$(CROSS)ld
#STRIP=$(CROSS)strip
SH_CFLAGS=-m4 -Wall -Os -fno-reorder-functions -fno-unit-at-a-time
PATCH_CFLAGS=$(SH_CFLAGS) -mnomacsave -fshort-wchar -nostartfiles -nostdlib
SOLIB_CFLAGS=$(SH_CFLAGS) -nostartfiles -nostdlib -shared
SOSTUB_CFLAGS=$(SOLIB_CFLAGS) -Lstub_stripped -lc_stub -lpthread_stub
OE_STAGE?=$(OE_BASE)/build/tmp/staging/sh4-linux
GLIBC_SRC?=$(OE_BASE)/build/tmp/work/sh4-linux/glibc-2.5*/GLIBC/glibc-2.5
INC=-I $(OE_STAGE)/include
LIB=-L $(OE_STAGE)/lib

all: inj-dbgin.elfpatch libc_stub.so libpthread_stub.so stub_stripped sym.so inj.so launcher.so

libc_stub.ld: exedsp.functions libc_vars.ld idafc2ld.awk
	@echo '/* Script for ld : dspEXE / libc */' > $@
	@echo 'SECTIONS' >> $@
	@echo '{' >> $@
	awk -f idafc2ld.awk libc=1 $< >> $@
	cat libc_vars.ld >> $@
	@echo '}' >> $@

libpthread_stub.ld: exedsp.functions idafc2ld.awk
	@echo '/* Script for ld : dspEXE / pthread */' > $@
	@echo 'SECTIONS' >> $@
	@echo '{' >> $@
	awk -f idafc2ld.awk pthread=1 $< >> $@
	@echo '}' >> $@

sym.ld: exedsp.functions exedsp.vars idafc2ld.awk
	@echo '/* Script for ld : dspEXE symbols */' > $@
	@echo 'SECTIONS' >> $@
	@echo '{' >> $@
	awk -f idafc2ld.awk pthread=2 libc=2 $< >> $@
	egrep -v ' _IO_| __environ' exedsp.vars >> $@
	@echo '}' >> $@

inj-dbgin.elfpatch: inj_stage1.c dbgMonInfoRet.s inj-dbgin.ld
	$(CC) $(PATCH_CFLAGS) -g -o $@ $(filter-out %.ld,$^) -Wl,-T$(filter %.ld,$^),-n

libc-internal: libc-internal.diff
	mkdir -p $@/bits
	cp $(GLIBC_SRC)/bits/libc-lock.h $@/bits
	cp $(GLIBC_SRC)/bits/linkmap.h $@/bits
	cp $(GLIBC_SRC)/sysdeps/generic/dl-lookupcfg.h $@
	cp $(GLIBC_SRC)/sysdeps/generic/dl-procinfo.c $@
	mkdir -p $@/elf
	cp $(GLIBC_SRC)/elf/link.h $@/elf
	cp $(GLIBC_SRC)/sysdeps/generic/hp-timing.h $@
	cp $(GLIBC_SRC)/sysdeps/generic/ldsodefs.h $@
	cp $(GLIBC_SRC)/include/libc-symbols.h $@
	cp $(GLIBC_SRC)/include/link.h $@
	patch -p0 < $<

libc_stub.so : syminit.c exedsp-abs.h libc_stub.ld libc-internal
	$(CC) $(SOLIB_CFLAGS) -Ilibc-internal -DSONAME=$(@:.so=) -o $@ $(filter %.c %.ld,$^)

libpthread_stub.so : syminit.c exedsp-abs.h libpthread_stub.ld libpthread_stub.map libc-internal
	$(CC) $(SOLIB_CFLAGS) -Ilibc-internal -DSONAME=$(@:.so=) -o $@ $(filter %.c %.ld,$^) -Wl,--version-script=$(filter %.map,$^)

sym.so : syminit.c exedsp-abs.h sym.ld libc-internal stub_stripped/libc_stub.so stub_stripped/libpthread_stub.so
	$(CC) $(SOSTUB_CFLAGS) -Ilibc-internal -DSONAME=$(@:.so=) -DMAKE_GLOBAL -o $@ $(filter %.c %.ld,$^) -Wl,-init,_initGlobalSym

stub_stripped :
	mkdir -p $@

stub_stripped/libc_stub.so stub_stripped/libpthread_stub.so :
	$(STRIP) -s -R .dynsym -o $@ $(@:stub_stripped/%=%)

inj.so : inj.c exedsp.h
	$(CC) $(SOSTUB_CFLAGS) -o $@ $<

launcher.so : launcher.c exedsp.h exedsp-abs.h
	$(CC) $(SOSTUB_CFLAGS) -o $@ $<

