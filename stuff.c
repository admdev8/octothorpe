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

#include <stdarg.h>
#include <stdio.h>
#include <search.h>
#include <stdlib.h>
#include <memory.h>
#include <ctype.h>

#include "oassert.h"
#include "datatypes.h"
#include "stuff.h"
#include "fmt_utils.h"
#include "ostrings.h"

#ifdef _MSC_VER
#include <intrin.h>
#endif

bool value_in(unsigned v, unsigned a1, unsigned a2, unsigned a3, unsigned a4, unsigned a5, unsigned a6, unsigned a7)
{
	return (v==a1) || (v==a2) || (v==a3) || (v==a4) || (v==a5) || (v==a6) || (v==a7);
};

unsigned most_significant_hex_number(octabyte x)
{
	octabyte t=x;
	int i;

	// _BitScanReverse64 can be used here, probably (?)

	for (i=0; i<(64/4); i++)
	{
		if (t&0xF000000000000000)
		{
			//printf (__func__"(0x%016llX) -> 0x%X\n", x, t>>(64-4));
			return t>>(64-4);
		}
		else
			t=t<<4;
	};
	//printf (__func__"(0x%016llX) -> 0\n", x);
	return 0;
};

_Noreturn void die (const char * fmt, ...)
{
	va_list va;
	va_start (va, fmt);

	vprintf (fmt, va);
	exit(0);
};

void* memdup (void *p, size_t s)
{
	void *rt=malloc (s);
	oassert(rt!=NULL);
	memcpy(rt, p, s);
	return rt;
};

void debugger_breakpoint()
{
#ifdef _MSC_VER
	__debugbreak();
#else
	__asm__("int $3");
#endif
};

FILE *fopen_or_die(const char* fname, const char* mode)
{
	FILE *rt=fopen (fname, mode);
	if (rt==NULL)
		die ("%s(): Can't open %s file in mode '%s'\n", __func__, fname, mode);
	return rt;
};

void make_REG_compact_hex (REG a, strbuf* out)
{
	if (a<10)
		strbuf_addf(out, PRI_SIZE_T_DEC, a);
	else
		strbuf_addf(out, "0x" PRI_SIZE_T_HEX, a);
};

// regs must be sorted before!
// if limit==0, then there are no limit
void make_compact_list_of_REGs (REG *regs, unsigned regs_total, strbuf *out, unsigned limit)
{
	if (limit>0 && regs_total>limit)
	{
		oassert (limit>=2);
		unsigned part_length=limit/2;
		make_compact_list_of_REGs (regs, part_length, out, 0);
		strbuf_addf (out, " (%d items skipped) ", regs_total-part_length*2);
		make_compact_list_of_REGs (regs+regs_total-part_length, part_length, out, 0);
		return;
	}

	for(unsigned i=0; i < regs_total; i++)
	{
		unsigned step=0;
		if (i+2<regs_total)
			for (step=8; step!=0; step=step>>1) // 8, 4, 2, 1
				if (regs[i]+step==regs[i+1] && regs[i+1]+step==regs[i+2])
				{
					unsigned i_start=i;
					REG n=regs[i];
					for (; n+step==regs[i+1] && i<regs_total; i++)
						n=regs[i+1];

					make_REG_compact_hex(regs[i_start], out);
					strbuf_addstr (out, "..");
					make_REG_compact_hex(regs[i], out);
					if (step>1)
						strbuf_addf (out, "(step=%d)", step);

					break;
				};

		if (step==0)
			make_REG_compact_hex(regs[i], out);

		if (i+1 < regs_total)
			strbuf_addstr (out, ", ");
	};
};

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

unsigned NULL_terminated_array_of_pointers_size(void **a)
{
	unsigned i;

	for (i=0; a[i]; i++);

	return i;
};

// this function was taken from http://www.blog.codereversing.com/infect4.pdf
unsigned align_to_boundary(unsigned address, unsigned boundary) 
{
	return ((address + boundary - 1) / boundary) * boundary;
};

// MIME stuff

struct string_pair
{
	const char *s1;
	const char *s2;
};

struct string_pair mime_types[]={     
	{ ".7z", "application/x-7z-compressed" },
	{ ".css", "text/css" },
	{ ".deb", "application/x-deb" },
	{ ".gif", "image/gif" },
	{ ".gz", "application/gzip" },
	{ ".htm", "text/html" },
	{ ".html", "text/html" },
	{ ".jpeg", "image/jpeg" },
	{ ".jpg", "image/jpeg" },
	{ ".js", "application/javascript" },
	{ ".mp3", "audio/mpeg" },
	{ ".mp4", "video/mp4" },
	{ ".ogg", "audio/ogg" },
	{ ".pdf", "application/pdf" },
	{ ".png", "image/png" },
	{ ".ps", "application/postscript" },
	{ ".rar", "application/x-rar-compressed" },
	{ ".tar", "application/x-tar" },
	{ ".txt", "text/plain" },
	{ ".xml", "text/xml" },
	{ ".asm", "text/x-asm" },
	{ ".c", "text/x-c" },
	{ ".c", "text/x-cpp" },
	{ ".zip", "application/zip" }
};

static int find_mime_type_by_filename (const void *filename, const void *struct_element)
{
	return string_is_ends_with (filename, ((const struct string_pair*)struct_element)->s1) ? 0 : 1;
};

const char *find_content_type_for_filename (const char *filename)
{
	size_t mime_types_size=sizeof(mime_types)/sizeof(struct string_pair);
	struct string_pair *found=lfind (filename, mime_types, &mime_types_size, sizeof(struct string_pair), find_mime_type_by_filename);
	const char *rt="application/octet-stream";

	if (found)
		rt=found->s2;

	//printf ("%s(%s) -> %s\n", __FUNCTION__, filename, rt);
	return rt;
};
/*
	printf ("%s\n", find_content_type_for_filename ("index.html"));
	printf ("%s\n", find_content_type_for_filename ("filename.ps"));
	printf ("%s\n", find_content_type_for_filename ("filename.pdf"));
	printf ("%s\n", find_content_type_for_filename ("filename.ext"));
*/

