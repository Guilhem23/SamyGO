Big thanks to aquadran for fixing/testing on different build envs.

Background Information:
	Build Cross-Compiler:
		http://www.ailis.de/~k/archives/19-ARM-cross-compiling-howto.html
	
	OpenEmbedded:
		http://en.wikipedia.org/wiki/Openembedded
		http://de.wikipedia.org/wiki/OpenEmbedded

	SamyGO:
		http://samygo.sourceforge.net

	Samsung:
		http://www.samsung.com

	SVN for Devs:
		http://svnbook.red-bean.com/en/1.1/svn-book.html

Usage Information:
	make -f Makefile-SELP-SamyGO [DL_DIR=<somedir>] <target>	

FAQ:
	Q: How long it takes to build the toolchain?
	A: Long (2-4 h), depends on your machine.
		(on a pentium4 3.2GHz)
		initial packages	~20 min.
		gcc-cross-initial	~20 min.
		linux-libc-headers	 ~8 min.
		glibc-intermediate	~30 min.
		gcc-cross		~17 min.
		glibc			~44 min. 
		linux-chelsea-2.6.18	~12 min.

	Build T-CHU7DEUC-image from clean svn checkout:
		~2:08:12 (no user interaction required / without download time for sources!)

	Q: Why i get an error on Mac Os X ...
	   when i try to compile glibc?
	A: http://crossgcc.rts-software.org/doku.php?id=i386linuxgccformac

	
