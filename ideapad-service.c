/******************************************************************************
 *
 * Allows acces to ideapad laptop features (fan mode, etc)
 *
 * Author: Tomas Pruzina <pruzinat@gmail.com>
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 ******************************************************************************/

#include <dbus/dbus.h>
#include <stdio.h>  //fprintf	
#include <stdlib.h> //exit

DBusError err;
DBusConnection* conn;
int ret;

int main()
{
	// get error iface
	dbus_error_init(&err);
	//connect to system bus
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);

	if (dbus_error_is_set(&err)) {
		  fprintf(stderr, "D-Bus connection Error (%s)\n", err.message);
		    dbus_error_free(&err);
	}
	
	if (!conn) {
		  fprintf(stderr, "D-Bus connection failed.\n");
		    exit(1);
	}

	ret = dbus_bus_request_name(conn, "ideapad.server", DBUS_NAME_FLAG_DO_NOT_QUEUE, &err);
	if (dbus_error_is_set(&err)) {
		  fprintf(stderr, "Getting bus name failed (%s)\n", err.message);
		    dbus_error_free(&err);
	}
	if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
		  fprintf(stderr, "Unable to became primary owner of the 'net.radekkrejci.server' bus name");
		    exit(1);
	}

	return 0;
}



