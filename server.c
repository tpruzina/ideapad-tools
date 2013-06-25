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

#define BUFFER 1024

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dbus/dbus.h>

#include "functions.h"
#include "bus.h"


void
reply_to_method_call(DBusMessage* msg, DBusConnection* conn)
{
	DBusMessage     *reply;
	DBusMessageIter  args;
	bool             stat = true;
	char            *reply_text = "Hello sender!";
	char            *param = "";
	/* unique number to associate replies with requests */
	dbus_uint32_t    serial = 0; 
	/* ideapad variables */
	struct data data;

	/* read the arguments */
	if (!dbus_message_iter_init(msg, &args))
		fprintf(stderr, "Message has no arguments!\n");
	else if (DBUS_TYPE_STRING != dbus_message_iter_get_arg_type(&args))
		fprintf(stderr, "Argument is not string!\n");
	else
		dbus_message_iter_get_basic(&args, &param);
	DEBUG_PRINT("Method called with arguments \"%s\"\n", param);

	/***************************************************************
	 ************* PARSE && PROCESS *******************************/
	if(!is_there_kernel_support())
		reply_text = "No kernel support!\n";
	else {
		/* parses string with arguments, saving into struct data */
		data = parse_buffer_into_struct(param);
		/* executes "jobs" and returns formated string that is to be sent back
		 * to client */
		reply_text = process_data(&data);
	}

	/************* DONE! ******************************************/

	/* create a reply from the message */
	reply = dbus_message_new_method_return(msg);
	
	/* add the arguments to the reply */
	dbus_message_iter_init_append(reply, &args);
	if (	!dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &stat)
		|| !dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &reply_text)){
		fprintf(stderr, "Out Of Memory!\n");
		exit(1);
	}

	/* send the reply && flush the connection */
	if (!dbus_connection_send(conn, reply, &serial)) { 
		fprintf(stderr, "Out Of Memory!\n");
		exit(1);
	}
	dbus_connection_flush(conn);
	
	/* free the reply */
	dbus_message_unref(reply);
}

int
main()
{
	DBusError err;
	DBusConnection* conn;
	int ret;
	DBusMessage     *msg;
	char            *filter_rule;

	/* initialise the errors */
	dbus_error_init(&err);

	/* connect to the bus */
	conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
	if (dbus_error_is_set(&err)) {
		fprintf(stderr, "D-Bus connection Error (%s)\n", err.message);
		dbus_error_free(&err);
	}
	if (NULL == conn) {
		fprintf(stderr, "D-Bus connection failed.\n");
		exit(1);
	}

	/* request a name on the bus */
	ret = dbus_bus_request_name(conn, SERVER_BUSNAME, DBUS_NAME_FLAG_DO_NOT_QUEUE, &err);
	if (dbus_error_is_set(&err))
		fprintf(stderr, "Getting bus name failed (%s)\n", err.message);

	if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret) {
		fprintf(stderr, "Unable to became primary owner of the '%s' bus name",
				SERVER_BUSNAME);
		exit(1);
	}

	/* add a rule for which messages we want to see */
	/* signals from the given sender and interface */
	filter_rule = (char *) malloc (BUFFER * sizeof (char));
	snprintf(filter_rule, BUFFER * sizeof(char), "type='signal', interface='%s'", CLIENT_SIGNAL_INTERFACE);
	dbus_bus_add_match(conn, filter_rule, &err);
	dbus_connection_flush(conn);
	if (dbus_error_is_set(&err)) {
		fprintf(stderr, "Match Error (%s)\n", err.message);
		exit(1); 
	}

	/* main loop, testing for new messages - 10*1000ms = 10 sec */
	unsigned char i=10;
	while(i--) {
		/* non blocking read of the next available message */
		dbus_connection_read_write(conn, 500);
		msg = dbus_connection_pop_message(conn);
		
		/* loop again if we haven't got a message */
		if (NULL == msg)
			continue;
		
		/* check this is a method call for the right interface and method
		 * or if it is a signal from the correct interface and with 
		 * the correct name
		 */
		if (dbus_message_is_method_call(msg, SERVER_METHOD_INTERFACE, "parse_cmds")) {
			reply_to_method_call(msg, conn);
		}

		/* free the message */
		dbus_message_unref(msg);
	}
	dbus_error_free(&err);
	free(filter_rule);

	return 0;
}
