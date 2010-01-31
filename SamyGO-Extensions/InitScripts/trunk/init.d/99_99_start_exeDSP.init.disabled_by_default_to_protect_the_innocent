#!/bin/sh
#
# © Copyright 1996-2010, ZsoltTech.Com
#       by Ser Lev Arris <arris@ZsoltTech.Com>
#
#       donated for the SamyGo Project
#       http://samygo.sourceforge.net/
#
#       Version: SamyGO svn $Id: 99_99_start_exeDSP.init.disabled_by_default_to_protect_the_innocent 334 2010-01-21 18:58:15Z arris $
#
# Untested!

. /dtv/SGO.env

[ -x "$SYSTOOT/bin/exeDSP" ] || exit 1

case $1 in 
	start)
	# avoid reboot 
	MicomCtrl 23
	killall -QUIT exeDSP
	# start alternate version of exeDSP
       	exeDSP	
	;;
	stop)
	# start the original exeDSP
	MicomCtrl 23
	killall -QUIT exeDSP
	cd /mtd_exe && ./exeDSP
	;;
	status)
	;;
	*)
	echo "Usage: $0 {start|stop}" 1>&2
	exit 0
	;;
esac
