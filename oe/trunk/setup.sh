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

DL_DIR=${DL_DIR:=$OE_BASE/sources}

# target platform: Chelsea (Leonid)
MACHINE=${MACHINE:=sdp83}

mkdir -p  ${OE_BASE}/build-${DISTRO}/conf

if [ ! -f ${OE_BASE}/build-${DISTRO}/conf/local.conf ] || [ ! -f ${OE_BASE}/build-${DISTRO}/env.source ] || [ "$1" = "--force" ]; then
	echo "DL_DIR = \"${DL_DIR}\"
OE_BASE = \"${OE_BASE}\"
BBFILES = \"\${OE_BASE}/oe/packages/*/*.bb \${OE_BASE}/oe/packages/apps-cl/*/*.bb\"
MACHINE = \"${MACHINE}\"
TARGET_OS = \"linux-gnueabi\"
DISTRO = \"${DISTRO}\"
CACHE = \"${OE_BASE}/cache/oe-cache.\${USER}\"
ASSUME_PROVIDED += \" gmp-native mpfr-native \"" > ${OE_BASE}/build-${DISTRO}/conf/local.conf

	if [ $OE_NUM_THREADS ] && [ $OE_NUM_THREADS -gt 1 ]; then
		echo "PARALLEL_MAKE = \"-j $OE_NUM_THREADS\"" >> ${OE_BASE}/build-${DISTRO}/conf/local.conf
	else
		echo "# PARALLEL_MAKE = \"-j 3\"" >> ${OE_BASE}/build-${DISTRO}/conf/local.conf
	fi

	echo "OE_BASE=\"${OE_BASE}\"
export BBPATH=\"\${OE_BASE}/oe/:\${OE_BASE}/bb/:\${OE_BASE}/build-${DISTRO}/\"
if [ ! \`echo \${PATH} | grep \${OE_BASE}/bb/bin\` ]; then
	export PATH=\${OE_BASE}/bb/bin:\${PATH}:\${OE_BASE}/build-${DISTRO}/tmp/cross/bin
fi
export LD_LIBRARY_PATH=
export PYTHONPATH=${OE_BASE}/bb/lib
export LANG=C" > ${OE_BASE}/build-${DISTRO}/env.source

echo "--- Created ${OE_BASE}/build-${DISTRO}/conf/local.conf and ${OE_BASE}/build-${DISTRO}/env.source ---"

fi

cd build-${DISTRO} && source env.source

echo
echo "--- SamyGO OE environment setup finished ---"
echo
echo "--- Usage example: bitbake samygo-externalboot-image ---"
echo

