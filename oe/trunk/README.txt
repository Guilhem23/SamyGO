
Background Information:
	Build Cross-Compiler:
		http://www.ailis.de/~k/archives/19-ARM-cross-compiling-howto.html
	
	OpenEmbedded:
		http://en.wikipedia.org/wiki/OpenEmbedded

	SamyGO:
		http://samygo.sourceforge.net

	Samsung:
		http://www.samsung.com

	SVN for Developers:
		http://svnbook.red-bean.com/en/1.1/svn-book.html


Usage Information:

	Run ". setup" to setup OE environment.
	Then use bitbake command.
	You may use below shell environment variables before run setup.
	Setup generate two configuration files:
	a) ${OE_BASE}/build/conf/local.conf
	b) ${OE_BASE}/build/env.source

	Once this two files are created you can setup directly from shell
	instead run setup.sh:
	${OE_BASE}/build; source env.source

Shell environment variables for setup OE:

	DL_DIR=<somedir>
		Directory to store caches source files.
		Default: $OE_BASE/sources

	MACHINE=<machine name>
		Target machine name.
		Default: sdp83

	OE_NUM_THREADS=<number of make threads>
		Number threads used for make command.
		Default: 1

	Example: MACHINE=sdp83;DISTRO=samygo;DL_DIR=~/sources;OE_NUM_THREADS=4;. setup.sh

FAQ:
	Q: Why I get an error on Mac OS X when I try to compile glibc?

	A: http://crossgcc.rts-software.org/doku.php?id=i386linuxgccformac

