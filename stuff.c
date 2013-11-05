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
#include <search.h>

#include "oassert.h"
#include "datatypes.h"
#include "stuff.h"
#include "fmt_utils.h"

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

void print_string_range (const char *s, int begin, size_t size)
{
	for (int i=0; i<size; i++)
		putc(s[begin+i], stdout);
};

char* str_trim_one_char_right (char *in)
{
	if (strlen(in)==0)
		return in;

	in[strlen(in)-1]=0;
	return in;
};

char str_last_char (const char *s)
{
	return s[strlen(s)-1];
};

char* str_trim_all_lf_cr_right (char *in)
{
	size_t slen=strlen(in);
	if (slen==0)
		return in;
	int last_char_pos=slen-1;

	if (in[last_char_pos]=='\n' || in[last_char_pos]=='\r')
	{
		in[last_char_pos]=0;
		str_trim_all_lf_cr_right(in);
	};

	return in;
};

char *remove_char_begin_end_if_present (char *s, char c)
{
	size_t slen=strlen(s);
	if (slen==0)
		return s;
	int last_char_pos=slen-1;
	if (s[0]==c && s[last_char_pos]==c)
	{
		s[last_char_pos]=0;
		strcpy (s, s+1);
	};
	return s;
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

int stricmp_range (const char *s1, int s1_begin, int s1_end, const char *s2)
{
	for (int i=s1_begin; i<s1_end; i++)
	{
		char c1=tolower(s1[i]), c2=tolower(s2[i]);
		if (c1!=c2)
			return c1-c2;
	};
	return 0;
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

#ifdef O_BITS64
octabyte strtol_or_strtoll(const char *nptr, char **endptr, int base)
#else
tetrabyte strtol_or_strtoll(const char *nptr, char **endptr, int base)
#endif    
{
#ifdef O_BITS64
	return strtoll(nptr, endptr, base);
#else
	return strtol(nptr, endptr, base);
#endif
};

const char *bool_to_string(bool b)
{
	if (b)
		return "true";
	return "false";
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

static int my_stricmp (const void *p1, const void *p2)
{
	//printf ("%s() p1=%s p2=%s\n", __FUNCTION__, (char*)p1, *(const char**)p2); // debug
	return stricmp (p1, *(const char**)p2);
};

static int my_strcmp (const void *p1, const void *p2)
{
	//printf ("%s() p1=%s p2=%s\n", __FUNCTION__, (char*)p1, *(const char**)p2); // debug
	return strcmp (p1, *(const char**)p2);
};

int find_string_in_array_of_strings(const char *s, const char **array, size_t array_size, 
	bool case_insensitive, bool sorted)
{
	void *found;
	
	if (sorted)
		found=bsearch (s, array, &array_size, sizeof(char*), case_insensitive ? my_stricmp : my_strcmp);
	else
		found=lfind (s, array, &array_size, sizeof(char*), case_insensitive ? my_stricmp : my_strcmp);

	if (found)
		return (const char**)found-array;
	else
		return -1; // string not found
};

const char *mon_name[12] = 
{
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

bool string_is_ends_with (const char *s, const char *ending)
{
	return strcmp (s+strlen(s)-strlen(ending), ending)==0 ? true : false;
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

