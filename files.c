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
 * Written by Dennis Yurichev <dennis(a)yurichev.com>, 2013-2017
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
#include <fcntl.h>

#include <sys/stat.h>

#include "datatypes.h"
#include "dmalloc.h"
#include "files.h"

#include "bitfields.h"
#include "stuff.h"
#include "oassert.h"

#ifdef __GNUC__
#include <unistd.h>
#endif

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

size_t get_file_size_or_die (const char* fname)
{
	FILE* f=fopen (fname, "rb");
	if (f==NULL)
		die ("Cannot open file %s\n", fname); // TODO: add errno, etc

	if (fseek (f, 0, SEEK_END)!=0)
		die ("fseek() failed\n");

	size_t fs=ftell (f);
	fclose (f);
	return fs;
};

byte* load_file_or_die (const char* fname, size_t *fsize /* can be NULL */)
{
	byte* rt;
	FILE* f;
	size_t fs;

	f=fopen (fname, "rb");
	if (f==NULL)
		die ("Cannot open file %s\n", fname); // TODO: add errno, etc

	fs=get_file_size_or_die(fname);
	//printf ("*fsize=%d\n", *fsize);
	rt=DMALLOC (byte, fs, "rt");

	if (fread (rt, fs, 1, f)!=1)
		die ("Cannot read file %s\n", fname); // TODO: add errno, etc

	fclose (f);
	if (fsize)
		*fsize=fs;
	return rt;
};

// ... or return NULL
byte* load_file (const char* fname, size_t *fsize /* can be NULL */)
{
	byte* rt;
	FILE* f;
	size_t fs;

	f=fopen (fname, "rb");
	if (f==NULL)
		return NULL;

	fs=get_file_size_or_die(fname);
	rt=DMALLOC (byte, fs, "rt");

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

// "filename.ext" -> "filename", "ext"
// no path!
void split_fname (char *fname, char *basefname, size_t basefname_len, char *ext, size_t ext_len)
{
	char *after_dot=strrchr (fname, '.')+1;
	oassert(after_dot!=NULL);

	// copy ext
	strncpy (ext, after_dot, ext_len);

	// copy base
	size_t out_basefname_len=after_dot - fname; // incl. zero
	oassert(out_basefname_len < basefname_len);
	memmove (basefname, fname, out_basefname_len-1);
	basefname[out_basefname_len-1]=0;
};

int open_or_die (char *fname, int mode)
{
	int fd=open(fname, mode);
	if (fd==-1)
		die ("Can't open file %s for read\n", fname);
	return fd;
};

FILE *fopen_or_die(const char* fname, const char* mode)
{
	FILE *rt=fopen (fname, mode);
	if (rt==NULL)
		die ("%s(): Can't open %s file in mode '%s'\n", __func__, fname, mode);
	return rt;
};

void my_truncate_or_die(char *fname, size_t newsize)
{
#ifdef __GNUC__
	truncate(fname, newsize);
	// TODO chk return
#endif	

#ifdef _MSC_VER
	int fd=open_or_die(fname, _O_RDWR);
	if (_chsize(fd, newsize)!=0)
		die ("Can't change size of %s\n", fname);
	_close(fd);
#endif
};

