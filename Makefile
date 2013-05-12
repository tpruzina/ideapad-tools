CC=gcc
DBUS_INCLUDES=-I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include

all: server client

client: ideapad-client.c
	$(CC) $(DBUS_INCLUDES) ideapad-client.c -o ideapad-client -ldbus-1

server: ideapad-server.c
	$(CC) $(DBUS_INCLUDES) ideapad-server.c -o ideapad-server -ldbus-1
