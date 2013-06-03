#!/usr/bin/env bash
umask 0
pushd `mvl-whereami`/../kerino/selp-ga-4.3.1.30-arm_v7_vfp_le-20091117/arm_v7_vfp_le/testsuite/binutils
usage()
{
    echo "

Description:
    Script to run cross binutils dejaGNU tests.

Usage:

     mv-test-cross-binutils.sh <target IP>  <target user name>

Examples:

     > mv-test-cross-binuitls.sh remote.example.com tester
       It will run the binutils tests on remote.example.com 
       with user 'tester'.
"
}

case $1 in
    --help | -h)
        usage
        exit 0
        ;;
    *)
        ;;
esac


if [ $# -ne 2 ]; then
    usage
    exit 0
fi

mkdir -p log
mkdir -p tmp
export DEJAGNU=$PWD/site.exp
pushd $PWD/testsuite
runtest TARGETIP=$1 REMOTEUSER=$2 --tool gas --all \
--outdir $PWD/../log --srcdir $PWD/gas \
--target_board=testboard --target armv7fl-montavista-linux-gnueabi \
-v
runtest TARGETIP=$1 REMOTEUSER=$2 --tool binutils --all \
--outdir $PWD/../log --srcdir $PWD/binutils \
--target_board=testboard --target armv7fl-montavista-linux-gnueabi \
-v
runtest TARGETIP=$1 REMOTEUSER=$2 --tool ld --all \
--outdir $PWD/../log --srcdir $PWD/ld \
--target_board=testboard --target armv7fl-montavista-linux-gnueabi \
-v
popd
echo "The test log files are in log."
popd
exit 0
