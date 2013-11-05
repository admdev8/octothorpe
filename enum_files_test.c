/*
 *             _        _   _                           
 *            | |      | | | |                          
 *   ___   ___| |_ ___ | |_| |__   ___  _ __ _ __   ___ 
 *  / _ \ / __| __/ _ \| __| '_ \ / _ \| '__| '_ \ / _ \
 * | (_) | (__| || (_) | |_| | | | (_) | |  | |_) |  __/
 *  \___/ \___|\__\___/ \__|_| |_|\___/|_|  | .__/ \___|
 *                                          | |         
 *                                          |_|
 *
 * Written by Dennis Yurichev <dennis(a)yurichev.com>, 2013
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/3.0/.
 *
 */

#include <stdio.h>
#include <time.h>

#include "enum_files.h"

void cb (const char *name, const char *pathname, size_t size, time_t t, bool is_dir, void *param)
{
	struct tm *_tm=localtime(&t);
	printf ("%s() name=%s pathname=%s size=%d is_dir=%d time=%s", 
		__FUNCTION__, name, pathname, size, is_dir, asctime(_tm));
};

int main()
{
	char tmp[256];
	enum_files_in_dir (getcwd (tmp, sizeof(tmp)), cb, NULL);
};
