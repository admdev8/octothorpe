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

#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

#ifdef _WIN32
#include <io.h>
#elif defined(__linux__) || defined (__CYGWIN__)
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#endif

#include "stuff.h"
#include "bitfields.h"
#include "strbuf.h"
#include "enum_files.h"

void enum_files_in_dir(const char* path, callback_fn cb, void *param)
{
#ifdef _WIN32
	strbuf tmp=STRBUF_INIT;
	strbuf_addf (&tmp, "%s\\*", path);
	struct _finddata_t d;
	int fh=_findfirst(tmp.buf, &d);
	assert(fh!=-1);
	strbuf tmp2=STRBUF_INIT;

	do
	{
		if (stricmp(d.name, ".")==0)
			continue;
		
		strbuf_reinit(&tmp2, 0);
		strbuf_addf (&tmp2, "%s\\%s", path, d.name);

		cb (d.name, tmp2.buf, d.size, d.time_write, IS_SET(d.attrib, _A_SUBDIR), param);

	} while (_findnext(fh, &d)==0);

	_findclose (fh);
	strbuf_deinit(&tmp2);
	strbuf_deinit (&tmp);
#elif defined(__linux__) || defined(__CYGWIN__)
	DIR* DIR_V = opendir(path);
	struct dirent* t = NULL;
	assert (DIR_V);

	strbuf tmp2=STRBUF_INIT;

	while ((t=readdir(DIR_V))!=NULL)
	{
		if (strcasecmp(t->d_name, ".")==0)
			continue;
		
		strbuf_reinit(&tmp2, 0);
		strbuf_addf (&tmp2, "%s/%s", path, t->d_name);
		struct stat st;
		if (stat (tmp2.buf, &st)==-1)
			die ("stat(%s) failed: %s\n", tmp2.buf, strerror(errno));
// conflict with c99...
#ifndef DT_DIR
#define DT_DIR 4
#endif
		cb (t->d_name, tmp2.buf, st.size, st.st_mtime, IS_SET(t->d_type, DT_DIR), param);
	};
	
	strbuf_deinit(&tmp2);
	closedir(DIR_V);
#endif
};
