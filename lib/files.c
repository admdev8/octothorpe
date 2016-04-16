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

// FIXME: all this stuff
#if defined(__linux__) || defined(__CYGWIN__) || defined(__APPLE__)
// for S_IFDIR macro, etc
// (conflict with -std=c99...)
#define _GNU_SOURCE
#endif

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
#if defined(__linux__) || defined(__CYGWIN__) || defined(__APPLE__)
	return IS_SET(buf.st_mode, S_IFREG);
#else
	return IS_SET(buf.st_mode, _S_IFREG); // win32
#endif
}

bool is_dir(const char* path) 
{
	struct stat buf;
	stat(path, &buf);
#if defined(__linux__) || defined(__CYGWIN__) || defined(__APPLE__)
	return IS_SET(buf.st_mode, S_IFDIR);
#else
	return IS_SET(buf.st_mode, _S_IFDIR); // win32
#endif
}

byte* load_file_or_die (const char* fname, size_t *fsize /* can be NULL */)
{
	byte* rt;
	FILE* f;
	size_t fs;

	f=fopen (fname, "rb");
	if (f==NULL)
		die ("Cannot open file %s\n", fname); // TODO: add errno, etc

	if (fseek (f, 0, SEEK_END)!=0)
		die ("fseek()\n");

	fs=ftell (f);
	//printf ("*fsize=%d\n", *fsize);
	rt=malloc (fs);

	if (fseek (f, 0, SEEK_SET)!=0)
		die ("fseek()\n");

	if (fread (rt, fs, 1, f)!=1)
		die ("Cannot read file %s\n", fname); // TODO: add errno, etc

	fclose (f);
	if (fsize)
		*fsize=fs;
	return rt;
};

// ... or return NULL
unsigned char* load_file (const char* fname, size_t *fsize /* can be NULL */)
{
	unsigned char* rt;
	FILE* f;
	size_t fs;

	f=fopen (fname, "rb");
	if (f==NULL)
		return NULL;

	if (fseek (f, 0, SEEK_END)!=0)
		return NULL;

	fs=ftell (f);
	rt=(unsigned char*)malloc (fs);

	if (fseek (f, 0, SEEK_SET)!=0)
		return NULL;

	if (fread (rt, fs, 1, f)!=1)
		return NULL;

	fclose (f);
	if (fsize)
		*fsize=fs;
	return rt;
};

void save_file_or_die (const char* fname, byte *buf, size_t fsize)
{
	FILE* f;

	f=fopen (fname, "wb");
	if (f==NULL)
		die ("Cannot open file for writing %s\n", fname); // TODO: add errno, etc

	if (fwrite (buf, fsize, 1, f)!=1)
		die ("Cannot write file %s\n", fname); // TODO: add errno, etc

	fclose (f);
};

void read_text_file_by_line_or_die (char *fname, read_text_file_by_line_callback_fn cb, void *param)
{
	FILE *f=fopen_or_die (fname, "rt");

	// FIXME: this is weird
	char fbuf[1024];
	while(fgets(fbuf, 1024, f)!=NULL)
		cb (fbuf, param);

	fclose (f);
};

