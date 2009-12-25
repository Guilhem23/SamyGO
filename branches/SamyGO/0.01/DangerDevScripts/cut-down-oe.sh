#!/bin/sh
# delete poss. unneeded packages from oe tree
# tripple check before use and make a backup copy from your 
# openembedded/packages directory!!


OE_DIR=$(pwd)/SamyGO

# build/tmp/work/
# openembedded/packages

for dirs in $OE_DIR/openembedded/packages/* ; do
	DIRe=$(basename $dirs)
	echo -e -n "Package $DIRe -> "
	MATCH=0
	for fuls in $OE_DIR/build/tmp/work/* addons tasks images wlan-Ralink dummypacks samsung libsdl ; do
		CH=$(basename $fuls)
		#	echo -e -n "\t $1 -> $CH -> $DIRe ->"
		echo $CH | grep -q "$DIRe"
		if [ $? -eq 0 ] ; then
			MATCH=1
			echo "protect $DIRe"			
			continue 2	
		fi
	done
	[ $MATCH ] &&  rm -rf $OE_DIR/openembedded/packages/$DIRe
done
