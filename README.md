ideapad-tools
=============

version: 0.1 <br>

command line utility which (will) support ideapad platform features (fan mode, rfkill...) <br>
Written to use recent (3.5+) kernel patches for ideapads (Z570..) <br>

Currently works:<br>
fan mode (silent,default, dust cleaning, effctive cooling) <br>
webcam power (on, off) <br> <br>
Todo: <br>
rfkill bluetooth, wifi <br>
backlight (should work out of the box) <br>
current fan speed <br>
<br>

Dependencies:<br>
dbus<br>
recent kernel with ideapad-laptop platform driver<br>

Usage:
<pre>
ideapad-client [OPTS]
OPTS:
--fan 			#prints current fan mode
 --fan silent		#sets fan mode to silent
 --fan default		#sets fan mode to default
 --fan dustclean	#sets fan mode to dust cleaning
 --fan cool		#sets fan mode to effective cooling

--webcam		#prints current webcam power status
 --webcam on		#turns webcam on
 --webcam off		#turns webcam off

--help			#this message
</pre>

Installation:<br>
<pre>
# make && make install
</pre>

Tree:
<pre>
.
├── bus.h
├── client.c
├── dbus
│   ├── ideapad.server.service
│   └── org.ideapad.conf
├── functions.c
├── functions.h
├── ideapad-client
├── ideapad-server
├── Makefile
├── platform.c
├── platform.h
├── README.md
├── server.c
└── todo

1 directory, 14 files
</tree>
