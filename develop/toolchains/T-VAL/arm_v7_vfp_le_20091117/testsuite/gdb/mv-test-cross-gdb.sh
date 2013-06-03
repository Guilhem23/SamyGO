#!/usr/bin/env bash
umask 0
pushd `mvl-whereami`/../devkit/arm/v7_vfp_le/testsuite/gdb/testsuite
usage()
{
    echo "

Description:
    Script to run cross GDB dejaGNU tests.

Usage:

     mv-test-cross-gdb.sh <target IP>  <target user name>

Examples:

     > mv-test-cross-gdb.sh remote.example.com tester
       It will run the gdb tests on remote.example.com 
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

mkdir -p ../log
mkdir -p tmp
export DEJAGNU=$PWD/../site.exp

id=1
while [ -e ../log/gdb.stdouterr.$id ]
do
        let id=$id+1
done

runtest TARGETIP=$1 REMOTEUSER=$2 --tool gdb \
--outdir $PWD/../log --srcdir $PWD \
--target_board=testboard --target armv7fl-montavista-linux-gnueabi \
--objdir $PWD --ignore fileio.exp 2>&1 |tee ../log/gdb.stdouterr.$id

popd
