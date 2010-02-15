LICENSE = "GPL"
DEPENDS = "zlib virtual/libsdl"
PR = "r01"

FILESPATH = "${FILE_DIRNAME}/qemu-${PV}"
FILESDIR = "${WORKDIR}"

SRC_URI = "\
	http://download.savannah.gnu.org/releases/qemu/qemu-${PV}.tar.gz \
#    http://www.bellard.org/qemu/qemu-${PV}.tar.gz \
"

S = "${WORKDIR}/qemu-${PV}"

OECONF = "--prefix=${prefix}/${TARGET_SYS} --enable-sdl --disable-kvm"
EXTRA_OECONF += "--target-list=arm-linux-user,arm-softmmu,sh4-linux-user,sh4-softmmu"
EXTRA_OECONF += "--disable-blobs --disable-strip --disable-system --disable-docs --disable-bsd-user --disable-linux-user --disable-darwin-user"

# +	# since we don't use the blobs feature, remove the '*.dtb' file (a broken
#+	# symlink) and manually copy in Makefile and *.dts, needed to build the few
#+	# parts of pc-bios used. 
#+	rm debian/system-build/pc-bios/*.dtb         
#+	cp pc-bios/Makefile pc-bios/*.dts debian/system-build/pc-bios/

inherit autotools

do_configure() {
    ./configure ${OECONF} ${EXTRA_OECONF}
}
