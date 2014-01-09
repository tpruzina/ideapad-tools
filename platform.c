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
#include <linux/version.h>

#include "platform.h"
#include "common.h" // debug print

int (*get_fan_state)(void);
int (*set_fan_state)(int state);
int (*get_camera_state)(void);
int (*set_camera_state)(int state);

int
platform_init(void)
{
	if(acpi_interface_present())
	{
		get_fan_state = acpi_get_fan_state;
		set_fan_state =	acpi_set_fan_state;
		get_camera_state = acpi_get_camera_state;
		set_camera_state = acpi_set_camera_state;
	}
	else if(sysfs_interface_present())
	{
		get_fan_state = sysctl_get_fan_state;
		set_fan_state =	sysctl_set_fan_state;
		get_camera_state = sysctl_get_camera_state;
		set_camera_state = sysctl_set_camera_state;
	}
	else
		return -1;
}

bool
sysfs_interface_present()
{
	return true;
}

bool
acpi_interface_present()
{
	return false;
}

/* FAN CONTROL VIA /sys interface */
int
sysctl_get_fan_state(void)
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
sysctl_set_fan_state(int state)
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
sysctl_get_camera_state(void)
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
sysctl_set_camera_state(int state)
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

/* ACPI INTERFACES */

int
acpi_get_fan_state(void)
{
}

int
acpi_set_fan_state(int state)
{
}

int
acpi_get_camera_state(void)
{
}

int
acpi_set_camera_state(int state)
{
}
