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

#ifndef _IDEAPAD_FUNCTIONS_H
#define _IDEAPAD_FUNCTIONS_H

#include <stdbool.h>

#define FAN_SILENT	0
#define FAN_STANDARD	1
#define	FAN_DUSTCLEAN	2
#define	FAN_DISSIPATION	4

struct data
{
	bool	print_fan;
	bool	set_fan;
	int	fan_val;

	bool	print_camera;
	bool	set_camera;
	int	camera_val;

};

char *parse_args_into_buffer(int argc, char **argv);
struct data parse_buffer_into_struct(char *buffer);
char *process_data(struct data *data);

void debug_print(const char *msg);

#endif

