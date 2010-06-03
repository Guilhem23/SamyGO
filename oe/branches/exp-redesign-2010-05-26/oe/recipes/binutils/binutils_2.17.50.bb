require binutils.inc
LICENSE = "GPLv3"

INC_PR = "r1"
PR = "${INC_PR}.0"

# SamyGO: workaround samsung download sources
FETCHCOMMAND_wget = "/usr/bin/env 'PATH=${PATH}' wget -t 5 --passive-ftp --no-check-certificate -P ${DL_DIR} \
	-O LA46B650.zip http://opensource.samsung.com/tv_n_video/la46b650/"
RESUMECOMMAND_wget = "/usr/bin/env 'PATH=${PATH}' wget -c -t 5 --passive-ftp --no-check-certificate -P ${DL_DIR} \
	-O LA46B650.zip http://opensource.samsung.com/tv_n_video/la46b650/"

SRC_URI = "http://opensource.samsung.com/tv_n_video/la46b650/LA46B650.zip \
	file://better_file_error.patch;patch=1 \
	file://ld_makefile.patch;patch=1 \
	file://detect-makeinfo.patch;patch=1"

do_unpack2() {
	tar --strip-components=2 -xvzf ${WORKDIR}/SELP.3.2.x-Chelsea.src.tgz SELP.3.2.x-Chelsea.src/Toolchain/binutils-2.17.50.tgz
	tar -xvzf binutils-2.17.50.tgz -C ${WORKDIR} || true
	rm binutils-2.17.50.tgz
	rm -f ${WORKDIR}/*.zip ${WORKDIR}/SELP* ${WORKDIR}/*.tgz ${WORKDIR}/*.gz ${WORKDIR}/README.txt || true
}

addtask unpack2 before do_patch after do_unpack

SRC_URI[md5sum] = "177f98d7a933f2de0c77b59241aaed34"
SRC_URI[sha256sum] = "154a15e19a2439e7d9a5d2cc431f4269c93fd08d1d31356be3579cf1243cc097"
