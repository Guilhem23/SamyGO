#!/bin/sh

# helper script for multimachine build

M="T-CHU7DEUC T-CHL7DEUC T-SPHAUSC T-RBYDEUC T-CHU7DAUC T-CHL5DEUC T-CHL5CIPDSEUC T-CHL6CIPDSEUC T-CHLCIPDEUC \
T-CHUCIPDEUC T-CHE6AUSC T-CHEAUSC T-CHE7AUSC T-CHL7DAUC T-CHEAEAC T-CHU7DAUC T-AMBAUSC \
T-RBYAUSC T-AMBDFRC T-CHE9AUSC T-CRLPEUHC T-CRLPEUWC T-AMBPEUMD T-PRLAUS"

M="T-CHU7DEUC T-SPHAUSC T-RBYDEUC T-PRLAUS"
# M="T-PRLAUS"

TARG="imaged"
TARG="SamyGO-Devel-image"
# TARG="SamyGO-KernelModules-image"

for i in $M ; do
	touch Makefile-SELP-SamyGO
#	make -f Makefile-SELP-SamyGO MACHINE=$i $TARG
	make -f Makefile-SELP-SamyGO MACHINE=$i initialize
#	make -f Makefile-SELP-SamyGO MACHINE=$i PatchFirm 
	cd build ; . ./env.source ; bitbake -v -D $TARG ; cd -
done
