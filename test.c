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

#include "platform.h"

#include <stdio.h>

int main()
{
	set_fan_state(0);
	set_camera_state(1);
	printf("%d %d",
		get_fan_state(),
		get_camera_state()
		);
	return 0;
}


