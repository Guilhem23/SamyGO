#! /bin/sh

PATH=/sbin:/bin:/usr/sbin:/usr/bin

rm /tmp/.X0-lock
Xvfb :0 -screen 0 960x540x24 &
x11vnc -24to32 -forever -nopw > /dev/null 2> /dev/null &
export DISPLAY=127.0.0.1:0
openbox &

: exit 0
