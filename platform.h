/******************************************************************************
 *
 * Small functions that write/read to /sys and handle some conditions
 *
 * Author: Tomas Pruzina <pruzinat@gmail.com>
 * 
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 ******************************************************************************/

#ifndef __IDEAPAD_PLATFORM_H__
#define __IDEAPAD_PLATFORM_H__

#define IFACE_DIR "/sys/devices/platform/ideapad"
#define FAN_CTRL_IFACE "/sys/devices/platform/ideapad/fan_mode"
#define CAMERA_CTRL_IFACE "/sys/devices/platform/ideapad/camera_power"

#define FAN_SILENT	0
#define FAN_STANDARD	1
#define	FAN_DUSTCLEAN	2
#define	FAN_DISSIPATION	4

int get_fan_state(void);
int set_fan_state(int state);

int get_camera_state(void);
int set_camera_state(int state);

#endif /* __IDEAPAD-FAN_H__ */

