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

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dbus/dbus.h>

#include "ideapad-dbus.h"
#include "ideapad-functions.h" // parsing functions

void
method_call(DBusConnection* conn, char *msg_text)
{
	DBusMessage	*msg, *reply;
	DBusMessageIter	args;
	DBusError	dbus_err;
	bool		stat;
	char 		*reply_text;
	/* initialise the errors */
	dbus_error_init(&dbus_err);

	/* create message */
	msg = dbus_message_new_method_call (
		SERVER_BUSNAME,           /* bus name */
		SERVER_OBJECT,            /* object path */
		SERVER_METHOD_INTERFACE,  /* interface */
		"parse_cmds");                /* method name - member */
	
	if (!msg) { 
		fprintf(stderr, "Preparing method call failed\n");
		exit(1);
	}
	
	/* append arguments */
	dbus_message_iter_init_append(msg, &args);
	if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &msg_text)) {
		fprintf(stderr, "OOM\n");
		exit(1);
	}

	/* send message and wait for reply */
	reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, &dbus_err);
	if (dbus_error_is_set (&dbus_err)) {
		/* handle DBUS_MESSAGE_TYPE_ERROR response message */
		fprintf(stderr, "Error response received:\n%s\n", dbus_err.message);
		exit(1);
	}
	
	/* cleanup */
	dbus_message_unref(msg);
	if (reply) {
		/* read the parameters */
		if (!dbus_message_iter_init(reply, &args))
			fprintf(stderr, "D-Bus message reply has no arguments");
		else if (DBUS_TYPE_BOOLEAN != dbus_message_iter_get_arg_type(&args))
			fprintf(stderr, "D-Bus message first argument is not boolean");
		else {
			dbus_message_iter_get_basic(&args, &stat);
	
			if (stat) { /* true value */
				fprintf(stdout, "Positive value from server received");
			} else { /* false value */
				fprintf(stdout, "Negative value from server recived");
			}
		}
	
		/* get the second parameter */
		if (!dbus_message_iter_next(&args))
			fprintf(stderr, "Message reply has too few arguments!\n");
		else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
			fprintf(stderr, "Argument is not string!\n");
		else {
			/* Get formatted reply message */
			
			dbus_message_iter_get_basic(&args, &reply_text);
			fprintf(stdout,"Reply message: %s\n", reply_text);
			
			/* *************************** */
		}
	
		/* free reply and close connection */
		dbus_message_unref (reply);
	} else
		fprintf(stderr, "No D-Bus response message received");
}


int
main(int argc, char *argv[])
{
	DBusError err;
	DBusConnection* conn;

	char *arg_buf = NULL;

	/* initialise the errors */
	dbus_error_init(&err);

	/* connect to the bus */
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
	if (dbus_error_is_set(&err)) {
		fprintf(stderr, "D-Bus connection Error (%s)\n", err.message);
		dbus_error_free(&err);
	}
	if (!conn) {
		fprintf(stderr, "D-Bus connection failed.\n");
		exit(1);
	}

	if (argc < 2)
		return 1;
	
	arg_buf = parse_args_into_buffer(argc,argv);
	
	method_call(conn, arg_buf);
	
	return 0;
}
