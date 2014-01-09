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
#include "common.h"

#include <stdio.h>
#include <assert.h>
#include <linux/version.h>


bool
test_linux_version()
{
	if(LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0))
		printf("\n Compile time kernel version is >= 3.13, acpi interface used.\n");
	else
		printf("\nCompile time kernel version is < 3.13, procfs fallback used.\n");
}

bool
test_opt_parse(char *test_str, struct data data)
{
	static int i=0;
	i++;
	struct data out;
	out = parse_buffer_into_struct(test_str);
	if(
		data.fan_print != out.fan_print	||
		data.fan_set != out.fan_set	||
		data.fan_val != out.fan_val 	||
		
		data.webcam_print != out.webcam_print	||
		data.webcam_set != out.webcam_set	||
		data.webcam_val != out.webcam_val
	)
	{
		fprintf(stderr, "[opt_parse_%d]\n"
				"%d %d %d\n"
				"%d %d %d\n",
				i,
				out.fan_print ? 1 : 0,
				out.fan_set ?	1 : 0,
				out.fan_val,
				
				out.webcam_print ? 1 : 0,
				out.webcam_set ?   1 : 0,
				out.webcam_val
		);
		return false;
	}
	else
	{
		printf("[opt_parse_%d] OK\n",i);
		return true;
	}
}

void
test_opt_parse2()
{
	test_opt_parse(
		"--fan ",
		(struct data){.fan_print = true, .fan_set = false});

	test_opt_parse(
		"--fana ",
		(struct data) {0});

	test_opt_parse(
		"--fan default",
		(struct data) { .fan_val = FAN_STANDARD, .fan_print = true,.fan_set = true});
	test_opt_parse(
		"--webcam off",
		(struct data) { .webcam_print = true, .webcam_set = true, .webcam_val = 0});
	test_opt_parse(
		"--webcam on",
		(struct data) { .webcam_print = true, .webcam_set = true, .webcam_val = 1});
	test_opt_parse(
		"--webcam on --fan dissipate",
		(struct data) { .webcam_print = true, .webcam_set = true, .webcam_val = 1,
				.fan_val = FAN_DISSIPATION, .fan_print = true,.fan_set = true });
}

void
test_platform_functs()
{
	/*
	set_fan_state(0);
	set_camera_state(1);
	printf("%d %d",
		get_fan_state(),
		get_camera_state()
	);
	*/
	test_linux_version();
}

int main()
{
	test_opt_parse2();
	test_platform_functs();
	return 0;
}


