CC=gcc
all: service

service: ideapad-server.c
	$(CC) -I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include ideapad-server.c -o ideapad-server -ldbus-1
