#!/usr/bin/env bash
umask 0
pushd `mvl-whereami`/../SELP.4.2.x/selp-ga-4.2.1.20-mips24ke_nfp_be-20090606/mips24ke_nfp_be/testsuite/gcc
usage()
{
    echo "

Description:
    Script to run cross GCC dejaGNU tests.

Usage:

     mv-test-cross-gcc.sh <target IP>  <target user name>

Examples:

     > mv-test-crossc.sh remote.example.com tester
       It will run the gcc tests on remote.example.com 
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

runtest TARGETIP=$1 REMOTEUSER=$2 --tool gcc --all \
--outdir $PWD/log --srcdir $PWD/gcc/testsuite \
--target_board=testboard --target mips-montavista-linux-gnu \
-v

runtest TARGETIP=$1 REMOTEUSER=$2 --tool g++ --all \
--outdir $PWD/log --srcdir $PWD/gcc/testsuite \
--target_board=testboard --target mips-montavista-linux-gnu \
-v

runtest TARGETIP=$1 REMOTEUSER=$2 --tool libstdc++ --all \
--outdir $PWD/log --srcdir $PWD/libstdc++-v3/testsuite \
--target_board=testboard --target mips-montavista-linux-gnu \
-v
./contrib/test_summary 2>&1 |tee log/summary
popd
echo "The test log files are in log."
exit 0
