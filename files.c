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
#include <stdlib.h>
#include <sys/stat.h>

#include "files.h"

#include "bitfields.h"
#include "stuff.h"

bool file_exist (const char *filename)
{
	FILE *tmp=fopen(filename, "r");
	if (tmp==NULL)
		return false;
	fclose (tmp);
	return true;
};

bool is_file(const char* path) 
{
	struct stat buf;
	stat(path, &buf);
	return IS_SET(buf.st_mode, _S_IFREG);
}

bool is_dir(const char* path) 
{
	struct stat buf;
	stat(path, &buf);
	return IS_SET(buf.st_mode, _S_IFDIR);
}

byte* load_file_or_die (const char* fname, size_t *fsize)
{
	byte* rt;
	FILE* f;

	f=fopen (fname, "rb");
	if (f==NULL)
		die ("Cannot open file %s\n", fname);

	if (fseek (f, 0, SEEK_END)!=0)
		die ("fseek()\n");

	*fsize=ftell (f);
	//printf ("*fsize=%d\n", *fsize);
	rt=malloc (*fsize);

	if (fseek (f, 0, SEEK_SET)!=0)
		die ("fseek()\n");

	if (fread (rt, *fsize, 1, f)!=1)
		die ("Cannot read file %s\n", fname);

	fclose (f);
	return rt;
};

unsigned char* load_file (const char* fname, size_t *fsize)
{
	unsigned char* rt;
	FILE* f;

	f=fopen (fname, "rb");
	if (f==NULL)
		return NULL;

	if (fseek (f, 0, SEEK_END)!=0)
		return NULL;

	*fsize=ftell (f);
	rt=(unsigned char*)malloc (*fsize);

	if (fseek (f, 0, SEEK_SET)!=0)
		return NULL;

	if (fread (rt, *fsize, 1, f)!=1)
		return NULL;

	fclose (f);
	return rt;
};

