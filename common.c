/******************************************************************************
 *
 * Functions common for both client and service or not belonging anywhere else
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
#include <dirent.h>


#include "common.h"
#include "platform.h"

/* client's command line arguments are parsed into formatted string which is to be sent
 * to server
 */
char *
parse_args_into_buffer(char *buffer,int argc, char **argv)
{
	*buffer='\0';
	while(--argc > 0 && ++argv){
		strncat(buffer,*argv,BUFFER_SIZE -1 - strlen(buffer));
		strncat(buffer," ", BUFFER_SIZE	-1 - strlen(buffer));
	}
	return buffer;
}

struct data
parse_buffer_into_struct(char *str)
{
	DEBUG_PRINT("Entering parse loop.");
	char *token = NULL;
	struct data data = {false,false,0,	//fan - print,set,value
			    false,false,0,};	//cam - print,set,value
	
	char *copy = (char *)malloc(strlen(str) + 1);
	if(!copy)
		return data;
	strcpy(copy,str);	

	token = strtok(copy," ");
	while(token && *token) {
		/* token was loaded, but not matched, try again */
		
		/* fan handling */
		if(strcmp("--fan",token) == 0) {
			/* we are going to print fan_mode value */
			data.fan_print = true;
			/* get next token */
			token = strtok(NULL," ");
			DEBUG_PRINT("--fan given");
			
			/* if we detect any of these, set property
			 * else just parse next token (if any)*/
			
			/* no more tokens -- print only */
			if(!token)
				continue;
			
			if(strcmp("silent",token) == 0)
				data.fan_val = FAN_SILENT;
			else if (strcmp("default",token) == 0)
				data.fan_val = FAN_STANDARD;
			else if(strcmp("dustclean",token) == 0)
				data.fan_val = FAN_DUSTCLEAN;
			else if(strcmp("dissipate",token) == 0)
				data.fan_val = FAN_DISSIPATION;
			else
				goto skip_write;		
			DEBUG_PRINT("--fan option detected");
			data.fan_set=true;
		
		/* webcam handling */
		} else if(strcmp("--webcam",token) == 0) {
			data.webcam_print = true;
			token = strtok(NULL, " ");
			DEBUG_PRINT("--webcam given");
			
			// no more tokens
			if(!token)
				continue;
			
			/* fan analogue, but we set on/off only */
			if(strcmp("on",token) == 0)
				data.webcam_val = 1;
			else if(strcmp("off",token) == 0)
				data.webcam_val = 0;
			else
				goto skip_write;
			DEBUG_PRINT("--webcam option detected");
			data.webcam_set = true;
		} else {
			/* Unrecognized option, return empty struct */
			break;
		}
		skip_write:
		/* get next token */
		token = strtok(NULL," ");
	}

	// cleanup tmp token
	free(copy);
	
	DEBUG_PRINT("Succesfully exiting parse loop");
	return data;
}

char *
process_data(struct data *data)
{
	DEBUG_PRINT("\nEntering processing function");
	/* main buffer for formatted message */
	char *reply_buffer = malloc(sizeof(char)*BUFFER_SIZE);
	if(!reply_buffer) {
		fprintf(stderr,"%s%d: OOM!\n",__FILE__,__LINE__);
		exit(1);
	}
	/* side buffer, used for snprintf and then concatenation to
	 * reply_buffer */
	// TODO: this is probably not necessary, use simple pointer?
	char partial_message[64] = {0};

	/* this may be useless, check standard/posix */
	*reply_buffer='\0';

	/* SAVE FAN MODE */
	if(data->fan_set)
		if(set_fan_state(data->fan_val) == -1) {
			DEBUG_PRINT("Could not have set fan mode!");
			data->fan_val = -1;
		}

	/* SAVE CAMERA MODE */
	if(data->webcam_set)
		if(set_camera_state(data->webcam_val) == -1) {
			DEBUG_PRINT("Could not have set webcam powerlevel!");
			data->webcam_val = -1;
		}
	/* PRINT CAMERA STATE */
	if(data->fan_print) {
		DEBUG_PRINT("%d\n",get_fan_state());
		/* Unless we did set fan, this is still -1 */
		if(!data->fan_set)
			data->fan_val=get_fan_state();

		sprintf(partial_message,
			"fan mode:\t%s\n", data->fan_val == 0 ? "silent" :
					  data->fan_val == 1 ? "default" :
					  data->fan_val == 2 ? "dust cleaning" :
					  data->fan_val == 4 ? "effective cooling" :
					  "ERROR!");
		strncat(reply_buffer,partial_message, BUFFER_SIZE - strlen(reply_buffer) - 1);
	}

	/* PRINT WEBCAM STATE */
	if(data->webcam_print) {
		DEBUG_PRINT("current camera state: %d",get_camera_state());
		/* unless we did set camera, this is still -1 */
		if(!data->webcam_set)
			data->webcam_val=get_camera_state();
		
		DEBUG_PRINT("camera state after: %d", get_camera_state());

		sprintf(partial_message,
			"camera power:\t%s\n",	data->webcam_val == 0 ? "off" :
						data->webcam_val == 1 ? "on" :
						"ERROR!");
		strncat(reply_buffer,partial_message, BUFFER_SIZE - strlen(reply_buffer) - 1);
	}

	/* Nothing was set --> error handling parameters */
	if(!data->webcam_print && !data->fan_print) {
		sprintf(partial_message,
			"Invalid command line parameters!\n");
		strncat(reply_buffer,partial_message, BUFFER_SIZE - strlen(reply_buffer) - 1);
	}

	DEBUG_PRINT("Successfully exiting processing function");

	/* Return buffer*/
	return reply_buffer;
}

bool
is_there_kernel_support()
{
	DIR *d = opendir(IFACE_DIR);
	/* /sys interface not found, probe loading module */
	if(!d) {
		system("modprobe ideapad-laptop");
		/* try again */
		d=opendir(IFACE_DIR);
		if(!d)
			return false;
	}
	closedir(d);
	return true;
}



