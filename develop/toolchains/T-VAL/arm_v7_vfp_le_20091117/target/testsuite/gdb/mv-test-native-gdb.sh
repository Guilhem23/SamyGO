#!/usr/bin/env bash
umask 0
pushd /testsuite/gdb/testsuite
mkdir -p ../log

id=1
while [ -e ../log/gdb.stdouterr.$id ]
do
        let id=$id+1
done

runtest --tool gdb --outdir ../log --srcdir $PWD GDB=/usr/bin/gdb 2>&1 |tee ../log/gdb.stdouterr.$id
popd

