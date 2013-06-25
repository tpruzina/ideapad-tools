/******************************************************************************
 *
 * Ideapad fan controls
 *
 * Author: Tomas Pruzina <pruzinat@gmail.com>
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 ******************************************************************************/
#include <stdio.h>

#include "platform.h"
#include "common.h" // debug print

/* FAN CONTROL VIA /sys interface */
int
get_fan_state(void)
{
	DEBUG_PRINT("Entering get_fan_state");
	int state=-1;
	FILE *fd = fopen(FAN_CTRL_IFACE,"r");
	if(!fd)
		return -1;
	fscanf(fd,"%d",&state);
	fclose(fd);
	return state;
}

int
set_fan_state(int state)
{
	DEBUG_PRINT("Entering set_fan_state, opt == %d",state);
	if(state < 0 || state > 4 || state == 3)
		return -1;
	FILE *fd = fopen(FAN_CTRL_IFACE, "w");
	if(!fd) return -1;
	fprintf(fd,"%d",state);
	fclose(fd);
	return state;
}

/* CAMERA CONTROL VIA /sys interface */
int
get_camera_state(void)
{
	DEBUG_PRINT("Entering get_camera_state");
	int state;
	FILE *fd = fopen(CAMERA_CTRL_IFACE,"r");
	if(!fd)
		return -1;
	fscanf(fd,"%d",&state);
	fclose(fd);
	return state;
}


int
set_camera_state(int state)
{
	DEBUG_PRINT("Entering set_camera_state, opt == %d",state);
	if(state < 0 || state > 1)
		return -1;
	FILE *fd = fopen(CAMERA_CTRL_IFACE, "w");
	if(!fd)
		return -1;
	fprintf(fd,"%d",state);
	fclose(fd);
	return state;
}

