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
#include <string.h>
#include <stdlib.h>

#include "ideapad-functions.h"


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


struct data
parse_buffer_into_struct(char *buffer)
{
	char *token = NULL;
	struct data data;

	/* todo parse empty string here?*/

	token = strtok(token," ");
	while(token && *token) {
		if(strcmp("--fan",token) == 0) {
			data.print_fan = true;
			token = strtok(NULL," ");
			
			if(strcmp("silent",token) == 0)
				data.fan_val = FAN_SILENT;
			else if (strcmp("standard",token) == 0)
				data.fan_val = FAN_STANDARD;
			else if(strcmp("dustclean",token) == 0)
				data.fan_val = FAN_DUSTCLEAN;
			else if(strcmp("cool",token) == 0)
				data.fan_val = FAN_DISSIPATION;
			else
				goto do_not_set_fan;
			
			data.set_fan=true;			
			do_not_set_fan:
			;
		}
		strtok(NULL," ");
	}
	return data;
}

char *
process_data(struct data *data)
{
	return "TEST";
}

/*
int main(int argc, char **argv)
{
	printf("%s\n",parse_args_into_buffer(argc, argv));
	return 0;
}
*/
