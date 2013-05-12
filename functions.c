/******************************************************************************
 *
 * TODO: Project Title
 *
 * Author: Tomas Pruzina <pruzinat@gmail.com>
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 ******************************************************************************/
#include <stdio.h>
#include <string.h> //strcmp,strtok
#include <stdlib.h> //malloc

#include "functions.h"
#include "platform.h"

/* client's command line arguments are parsed into formatted string which is to be sent
 * to server
 */
#define BUFFER_SIZE 128
char *
parse_args_into_buffer(int argc, char **argv)
{
	char *buffer = malloc(sizeof(BUFFER_SIZE));
	*buffer='\0';
	while(--argc > 0 && ++argv){
		strncat(buffer,*argv,BUFFER_SIZE - strlen(buffer) - 1);
		strncat(buffer," ", BUFFER_SIZE - strlen(buffer) - 1);
	}
	return buffer;
}


#define DEBUG 1
struct data
parse_buffer_into_struct(char *buffer)
{
	char *token = buffer;
	struct data data = {false,false,-1,	//fan - print,set,value
			    false,false,-1,};	//cam - print,set,value

	/* todo parse empty string here?*/

	token = strtok(token," ");
	while(token && *token) {
		/* token was loaded, but not matched, try again */
		beginning:
		
		/* fan handling */
		if(strcmp("--fan",token) == 0) {
			/* we are going to print fan_mode value */
			data.fan_print = true;
			/* get next token */
			token = strtok(NULL," ");
			debug_print("--fan given\n");
			/* if we detect any of these, set property
			 * else just parse next token (if any)*/
			if(strcmp("silent",token) == 0)
				data.fan_val = FAN_SILENT;
			else if (strcmp("default",token) == 0)
				data.fan_val = FAN_STANDARD;
			else if(strcmp("dustclean",token) == 0)
				data.fan_val = FAN_DUSTCLEAN;
			else if(strcmp("effective",token) == 0)
				data.fan_val = FAN_DISSIPATION;
			else
				goto beginning;
			debug_print("--fan option detected");
			data.fan_set=true;			
		}
		/* webcam handling */
		if(strcmp("--webcam",token) == 0) {
			data.webcam_print = true;
			token = strtok(NULL, " ");
			debug_print("--webcam given");
			/* fan analogue, but we set on/off only */
			if(strcmp("on",token) == 0)
				data.webcam_val = 1;
			else if(strcmp("off",token) == 0)
				data.webcam_val = 0;
			else
				goto beginning;
			debug_print("--webcam option detected");
			data.webcam_set = true;
		}
		/* get next token */
		token = strtok(NULL," ");
	}
	debug_print("exited parse loop\n");
	return data;
}

char *
process_data(struct data *data)
{
	char *reply_buffer = malloc(sizeof(BUFFER_SIZE));
	/* Handle alloc error */

	*reply_buffer='\0';

	if(data->fan_set)
		if(set_fan_state(data->fan_val) == -1)
			debug_print("Could not have set fan mode!");
	if(data->webcam_set)
		if(set_camera_state(data->webcam_val) == -1)
			debug_print("Could not have set webcam powerlevel!");

	if(data->fan_print) {
		printf("%d\n",get_fan_state());
	}
	if(data->webcam_print) {
		printf("%d\n",get_camera_state());
	}
	return reply_buffer;
}

#ifndef DEBUG
void debug_print(const char *msg)
{
	;
}
#endif 

#ifdef DEBUG
debug_print(const char *msg)
{
	fprintf(stderr, "%s:%i: %s\n", __FILE__, __LINE__, msg);
}
#endif

