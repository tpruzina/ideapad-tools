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

#define BUFFER_SIZE 512

#define FAN_SILENT	0
#define FAN_STANDARD	1
#define	FAN_DUSTCLEAN	2
#define	FAN_DISSIPATION	4

struct data
{
	bool	fan_print;
	bool	fan_set;
	int	fan_val;

	bool	webcam_print;
	bool	webcam_set;
	int	webcam_val;

};

char *parse_args_into_buffer(char *buffer,int argc, char **argv);
struct data parse_buffer_into_struct(char *buffer);
char *process_data(struct data *data);
bool is_there_kernel_support(void);

#ifdef DEBUG
#define DEBUG_PRINT(...) do{ \
			fprintf(stderr, "%s:%i:",__FILE__, __LINE__);\
			fprintf(stderr,__VA_ARGS__ );\
			(void) fflush(stderr);\
			} while( false )
#else
#define DEBUG_PRINT(...) do{ ; } while ( false )
#endif


#endif

