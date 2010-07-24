#!/bin/sh

is_root=`id | grep root`
if [ "$is_root" ]; then
	echo
	echo "* ERROR * You are with 'root' privileges ! Aborting."
	echo
	exit 1
fi

if [ $0 == "./setup.sh" ]; then
	echo
	echo "* ERROR * You must run via '. setup.sh'"
	echo
	exit 1
fi

OE_BASE=`pwd`
export OE_BASE=`readlink -f $OE_BASE`

DISTRO=${DISTRO:=samygo}
DL_DIR=${DL_DIR:="$HOME/sources"}

# target platform: ssdtv
MACHINE=${MACHINE:=ssdtv}

mkdir -p  ${OE_BASE}/build-${DISTRO}/conf

BBF="\${OE_BASE}/oe/recipes/*/*.bb"
if [ "${DISTRO}" = "samygo-cl" ]; then
	BBF="${BBF} \${OE_BASE}/oe/recipes/apps-cl/*/*.bb"
fi

if [ ! -f ${OE_BASE}/build-${DISTRO}/conf/local.conf ] || [ ! -f ${OE_BASE}/build-${DISTRO}/env.source ] || [ "$1" = "--force" ]; then
	echo "DL_DIR = \"${DL_DIR}\"
OE_BASE = \"${OE_BASE}\"
BBFILES = \"${BBF}\"
MACHINE = \"${MACHINE}\"
TARGET_OS = \"linux-gnueabi\"
DISTRO = \"${DISTRO}\"
INHERIT = \"rm_work\"
IMAGE_KEEPROOTFS = \"1\"
CACHE = \"${OE_BASE}/build-${DISTRO}/cache/oe-cache.\${USER}\"
ASSUME_PROVIDED += \" git-native perl-native libxml-parser-perl-native desktop-file-utils-native linux-libc-headers-native \"
PARALLEL_MAKE = \"-j 2\"
#BB_NUMBER_THREADS = \"2\"" > ${OE_BASE}/build-${DISTRO}/conf/local.conf

	echo "OE_BASE=\"${OE_BASE}\"
export BBPATH=\"\${OE_BASE}/oe/:\${OE_BASE}/bb/:\${OE_BASE}/build-${DISTRO}/\"
if [ ! \`echo \${PATH} | grep \${OE_BASE}/bb/bin\` ]; then
	export PATH=\${OE_BASE}/bb/bin:\${PATH}:\${OE_BASE}/build-${DISTRO}/tmp/cross/armv6/bin
fi
export LD_LIBRARY_PATH=
export PYTHONPATH=${OE_BASE}/bb/lib
export LANG=C" > ${OE_BASE}/build-${DISTRO}/env.source

echo "--- Created ${OE_BASE}/build-${DISTRO}/conf/local.conf and ${OE_BASE}/build-${DISTRO}/env.source ---"

fi

bitbake() {
	cd ${OE_BASE}/build-${DISTRO} && source env.source && ${OE_BASE}/bb/bin/bitbake $@
}

echo
echo "--- SamyGO OE configuration finished ---"
echo
if [ "${DISTRO}" = "samygo-cl" ]; then
echo "--- Usage example: bitbake scummvm-cl ---"
else
echo "--- Usage example: bitbake externalboot-base ---"
fi
echo

