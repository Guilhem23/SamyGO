#!/bin/sh

# helper script to clean targets/recipe

M="T-CHU7DEUC T-CHL7DEUC T-SPHAUSC T-RBYDEUC T-CHU7DAUC T-CHL5DEUC T-CHL5CIPDSEUC T-CHL6CIPDSEUC T-CHLCIPDEUC \
T-CHUCIPDEUC T-CHE6AUSC T-CHEAUSC T-CHE7AUSC T-CHL7DAUC T-CHEAEAC T-CHU7DAUC T-AMBAUSC \
T-RBYAUSC T-AMBDFRC T-CHE9AUSC T-CRLPEUHC T-CRLPEUWC T-AMBPEUMD T-PRLAUS"

TARG="imaged"
TARG="SamyGO-Devel-image"
TARG="SamyGO-KernelModules-image"
PACKS="gcc gcc-cross gcc-cross-initial glibc-initial glibc glibc-intermediate"
PACKS="virtual/kernel"
PACKS="samsung-firmware"

for i in $M ; do
	touch Makefile-SELP-SamyGO
	#	make -f Makefile-SELP-SamyGO MACHINE=$i $TARG
	make -f Makefile-SELP-SamyGO MACHINE=$i initialize
	for j in $PACKS ; do
		#	cd build ; . ./env.source ; bitbake -v -D $TARG ; cd -
		cd build ; . ./env.source ; bitbake -c clean $j ; cd -
	done
done
