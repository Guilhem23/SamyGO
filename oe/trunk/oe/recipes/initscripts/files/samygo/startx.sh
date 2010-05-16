#! /bin/sh

PATH=/sbin:/bin:/usr/sbin:/usr/bin

Xvfb :0 -screen 0 1280x760x24 &
x11vnc &
openbox &
xterm &

: exit 0
