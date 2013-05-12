CC=gcc
all: service

service: ideapad-service.c
	$(CC) -I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include ideapad-service.c -o ideapad-server -ldbus-1
