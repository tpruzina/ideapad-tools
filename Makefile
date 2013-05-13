CC=gcc
DBUS_INCLUDES=-I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include
CFLAGS=-fno-stack-protector -g -std=gnu99 -Wall -DDEBUG
PREFIX=/usr/local

all: server client

client: functions.c client.c platform.c
	$(CC) $(DBUS_INCLUDES) $(CFLAGS) $^ -o ideapad-client -ldbus-1

server: platform.c functions.c server.c
	$(CC) $(DBUS_INCLUDES) $(CFLAGS) $^ -o ideapad-server -ldbus-1

install: all
	cp ./ideapad-client $(PREFIX)/bin/
	cp ./ideapad-server $(PREFIX)/sbin/
	cp ./dbus/org.ideapad.conf /etc/dbus-1/system.d/
	cp ./dbus/ideapad.server.service /usr/share/dbus-1/system-services/

uninstall:
	rm -f $(PREFIX)/bin/ideapad-client
	rm -f $(PREFIX)/sbin/ideapad-server
	rm -f /etc/dbus-1/system.d/org.ideapad.conf
	rm -f /usr/share/dbus-1/system-services/ideapad.server.service

