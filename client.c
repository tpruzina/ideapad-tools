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

#include "bus.h"
#include "common.h" // parsing functions

void
method_call(DBusConnection *conn, const char *msg_text)
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
		fprintf(stderr, "Error!\n%s\n", dbus_err.message);
		exit(1);
	}
	dbus_error_free(&dbus_err);
	
	/* cleanup */
	dbus_message_unref(msg);
	
	if (reply) {
		/* read the parameters */
		if (!dbus_message_iter_init(reply, &args))
			fprintf(stderr, "D-Bus message reply has no arguments");
		else if (DBUS_TYPE_BOOLEAN != dbus_message_iter_get_arg_type(&args))
			fprintf(stderr, "D-Bus message first argument is not boolean");
		else
			dbus_message_iter_get_basic(&args, &stat);
	
		/* get the second parameter */
		if (!dbus_message_iter_next(&args))
			fprintf(stderr, "Message reply has too few arguments!\n");
		else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
			fprintf(stderr, "Argument is not string!\n");
		else {
			/* Get formatted reply message */
			dbus_message_iter_get_basic(&args, &reply_text);
			/* PRINT OUTPUT FROM SERVER */
			printf("%s", reply_text);
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

	if(argc == 1 || (argc == 2 && strcmp("--help",argv[1]) == 0)) {
		printf("Usage:"
			"ideapad-client <OPTS>\n"
			"OPTS:\n"
			"--fan 			#prints current fan mode\n"
			" --fan silent		#sets fan mode to silent\n"
			" --fan default		#sets fan mode to default\n"
			" --fan dustclean	#sets fan mode to dust cleaning\n"
			" --fan dissipate		#sets fan mode to effective cooling\n\n"
			"--webcam		#prints current webcam power status\n"
			" --webcam on		#turns webcam on\n"
			" --webcam off		#turns webcam off\n\n"
			"--help			#this message\n"
		);
		return 0;
	}

	char *arg_buf = NULL;

	/* initialise the errors */
	dbus_error_init(&err);

	/* connect to the bus */
	conn = dbus_bus_get_private(DBUS_BUS_SYSTEM, &err);
	if (dbus_error_is_set(&err))
		fprintf(stderr, "D-Bus connection Error (%s)\n", err.message);
	
	if (!conn) {
		fprintf(stderr, "D-Bus connection failed.\n");
		exit(1);
	}

	if (argc < 2)
		return 1;
	
	/* Allocates buffer for dbus message string */
	arg_buf = malloc(BUFFER_SIZE);
	/* Parses command line arguments into string array */
	parse_args_into_buffer(arg_buf,argc,argv);
	/* Send message <param array> to server */
	method_call(conn, arg_buf);
	/* Cleanup */
	free(arg_buf);
	
	dbus_connection_unref(conn);
	dbus_error_free(&err);

	return 0;
}
