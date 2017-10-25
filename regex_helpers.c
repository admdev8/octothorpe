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
 * Written by Dennis Yurichev <dennis(a)yurichev.com>, 2016
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/3.0/.
 *
 */

//#include <stdarg.h>
//#include <stdio.h>
//#include <search.h>
//#include <stdlib.h>
//#include <memory.h>
//#include <ctype.h>

//#include "oassert.h"
//#include "datatypes.h"
//#include "stuff.h"
//#include "fmt_utils.h"
//#include "ostrings.h"
#include "dmalloc.h"

//#ifdef _MSC_VER
//#include <intrin.h>
//#endif

#include "regex.h"

void regcomp_or_die (regex_t *_Restrict_ preg, const char *_Restrict_ pattern, int cflags)
{
	int rc=regcomp(preg, pattern, cflags);
	if (rc!=_REG_NOERROR)
	{
		char buffer[100];
		regerror(rc, preg, buffer, 100);
		die("Regular expression compiling failed for pattern '%s' (%s)", pattern, buffer);
	};
};

size_t regmatch_len (regmatch_t *m)
{
	return m->rm_eo - m->rm_so;
};

char *regmatch_dup(regmatch_t *m, char *str)
{
	if (m==NULL || m->rm_so<0)
		return NULL;

	return DSTRNDUP (str + m->rm_so, regmatch_len(m), "str");
};

// untested
char **regexec_to_array_of_string (regex_t *r, char *s, size_t nmatch)
{
	regmatch_t *m=DMALLOC(regmatch_t, nmatch, "regmatch_t");

	if (regexec(r, s, nmatch, m, 0))
	{
		DFREE(m);
		return NULL;
	};

	char **rt=DMALLOC(char*, nmatch+1, "char*");

	for (size_t i=0; i<nmatch; i++)
		rt[i]=regmatch_dup(&m[i], s);

	rt[nmatch]=NULL; // terminator

	DFREE(m);
	return rt;
};

