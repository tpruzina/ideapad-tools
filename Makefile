DBUS_INCLUDES=`pkg-config --cflags --libs dbus-1`
CFLAGS=-O0 -g -DDEBUG
PREFIX=/usr/local

all: ideapad-client ideapad-server test

ideapad-client: common.c client.c platform.c
	$(CC) $(CFLAGS) $(DBUS_INCLUDES) $^ -o $@ 
ideapad-server: server.c common.c platform.c
	$(CC) $(CFLAGS) $(DBUS_INCLUDES) $^ -o $@ 

install:
	cp ./dbus/org.ideapad.server.service	/usr/share/dbus-1/system-services/
	cp ./dbus/org.ideapad.conf		/etc/dbus-1/system.d/
	cp ./ideapad-client			$(PREFIX)/bin/
	cp ./ideapad-server			$(PREFIX)/sbin/

uninstall:
	rm -f /usr/share/dbus-1/system-services/org.ideapad.server.service
	rm -f /etc/dbus-1/system.d/org.ideapad.conf
	rm -f $(PREFIX)/bin/ideapad-client
	rm -f $(PREFIX)/sbin/ideapad-server

test: test.c common.c platform.c
	$(CC) $(CFLAGS) $^ -o $@ 

.PHONY: clean

clean:
	rm -f ./ideapad-client ./ideapad-server ./test
