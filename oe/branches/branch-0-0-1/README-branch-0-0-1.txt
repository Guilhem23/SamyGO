Big thanks to aquadran for fixing/testing on different build envs.

This is the testing branch for toolchains (you won't find here any usable applications for your samy).
If you are using this svn branch report errors to http://sourceforge.net/apps/phpbb/samygo/viewforum.php?f=4

Background Information:
	Build Cross-Compiler:
		http://www.ailis.de/~k/archives/19-ARM-cross-compiling-howto.html
	
	OpenEmbedded:
		http://wiki.openembedded.org/index.php/OEandYourDistro
		http://en.wikipedia.org/wiki/Openembedded
		http://de.wikipedia.org/wiki/OpenEmbedded

	SamyGO:
		http://samygo.sourceforge.net

	Samsung:
		http://www.samsung.com

	SVN for Devs:
		http://svnbook.red-bean.com/en/1.1/svn-book.html

Usage Information:
	make -f Makefile-SELP-SamyGO [DL_DIR=<somedir>] [MACHINE=<machine>] <target>
		(it's safer to set the MACHINE direct in the Makefile)
	Common targets:
		Toolchain
		image
		static-image
		PatchFirm
		gallery
				
	Available machines:
		T-CHU7DEUC	(default)
		T-SPHAUSC
		T-RBYDEUC	

Qemu:
	You can test your binaries:
		generate one of the "image" targets and qemu-native, or
		...

		SH4: ./build/tmp/staging/i686-linux/bin/qemu-sh4 build/tmp/rootfs/lib/ld-linux.so.2 --library-path "build/tmp/rootfs/lib" build/tmp/rootfs/bin/ps

		ARM: ./build/tmp/staging/i686-linux/bin/qemu-arm build/tmp/rootfs/lib/ld-linux.so.3 --library-path "build/tmp/rootfs/lib" build/tmp/rootfs/bin/ps


Status Information:
	T-CHU7DEUC: arm
		Toolchain: 
			build	y
			usable	y
		Kernel:
			build	y
			usable	?
		Modules:
			build	y
			usable	y
		u-boot:
			build	y
			usable	?
	T-SPHAUSC: arm
		Toolchain: 
			build	y
			usable	y 
		Kernel:
			build	y
			usable	?
		Modules:
			build	y
			usable	y
		u-boot:
			build	y
			usable	?
	T-RBYDEUC: sh4
		Toolchain:
			build	y
			usable	y
		Kernel:
			build	y
			usable	y
		Modules:
			build	y
			usable	y
		u-boot:
			build	y
			usable	? not any of b650/750 version?

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

	Build SamyGO-image (for T-CHU7DEUC) from clean svn checkout:
		~2:08:12 (no user interaction required / without download time for sources!)

	Q: Why i get an error on Mac Os X ...
	   when i try to compile glibc?
	A: http://crossgcc.rts-software.org/doku.php?id=i386linuxgccformac

Known BUGS:
	* not striped kernel-modules will be packaged -> fixed
	* lot of c99 warnings on busybox build
	* sh4 wrong gcc used on kernel-module build
		fixed for fuse-module (CC=KERNEL_CC)

