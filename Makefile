CC=gcc
DBUS_INCLUDES=-I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include
CFLAGS=-g -std=gnu99 -Wall


all: server client

client: functions.c client.c platform.c
	$(CC) $(DBUS_INCLUDES) $(CFLAGS) $^ -o ideapad-client -ldbus-1

server: platform.c functions.c server.c
	$(CC) $(DBUS_INCLUDES) $(CFLAGS) $^ -o ideapad-server -ldbus-1
