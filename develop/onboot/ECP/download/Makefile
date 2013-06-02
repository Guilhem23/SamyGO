SHELL	= /bin/sh
#CC	= gcc -Wformat -Wunused
CC = gcc -Wformat -Wunused
CROSSCC2 =arm_v7_vfp_le
OPT	= -O
DEBUG	= -g
PROGS	= com
CFLAGS	= $(OPT) $(DEBUG) -I. $(XFLAGS) $(DEFS)
OBJSD	= com.o
OBJSE	= com.o
PROGS	= com
MAN	= com.1

what:
	@sh makedefs

all:	$(PROGS)

com:	
	gcc -Wunused -O -g -I.  -DLINUX2   -c -o com.o com.c
	gcc $(CFLAGS) -o $@ $(OBJSD) $(SYSLIBS) -ldl



clean:
	rm -f $(PROGS) *.o core *.core

depend: $(MAKES)
	(sed '1,/^# do not edit/!d' Makefile; \
	set -e; for i in [a-z][a-z0-9]*.c; do \
	    $(CC) -E $(DEFS) $(INCL) $$i | sed -n -e '/^# *1 *"\([^"]*\)".*/{' \
	    -e 's//'`echo $$i|sed 's/c$$/o/'`': \1/' -e 'p' -e '}'; \
	done) | grep -v '[.][o][:][ ][/]' >$$$$ && mv $$$$ Makefile


