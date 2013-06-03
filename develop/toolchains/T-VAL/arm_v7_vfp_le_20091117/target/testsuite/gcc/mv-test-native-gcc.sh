#!/usr/bin/env bash
umask 0
pushd /testsuite/gcc
mkdir -p log
runtest --all --tool gcc --outdir log --srcdir gcc/testsuite CC=gcc CXX=g++ HOSTCC=gcc HOSTCFLAGS="" -v
runtest --all --tool g++ --outdir log --srcdir gcc/testsuite CC=gcc CXX=g++ HOSTCC=gcc HOSTCFLAGS="" -v
runtest --all --tool libstdc++ --outdir log --srcdir libstdc++-v3/testsuite CC=gcc CXX=g++ HOSTCC=gcc HOSTCFLAGS="" -v
./contrib/test_summary 2>&1 |tee log/summary
popd
echo "The test log files are in log."
exit 0
