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
#include "dmalloc.h"

#ifdef _MSC_VER
#include <intrin.h>
#endif

// for lzcnt:
#include <x86intrin.h>

bool value_in2(unsigned v, unsigned a1, unsigned a2)
{
	return (v==a1) || (v==a2);
};

bool value_not_in2(unsigned v, unsigned a1, unsigned a2)
{
	return !((v==a1) || (v==a2));
};

bool value_in7(unsigned v, unsigned a1, unsigned a2, unsigned a3, unsigned a4, unsigned a5, unsigned a6, unsigned a7)
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

// NULL-terminated array
unsigned NULL_terminated_array_of_pointers_size(void **a)
{
	unsigned i;

	for (i=0; a[i]; i++);

	return i;
};

// NULL-terminated array
void print_array_of_strings (char **s)
{
	while (*s)
		printf ("%s\n", *s++);
};

// NULL-terminated array
void dfree_array_of_blocks (char **s)
{
	char **tmp=s;

	while (*s)
		DFREE (*s++);
	
	DFREE (tmp);
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

#define CRC32POLY 0xEDB88320ul

static tetrabyte CRC32_table[256];
static bool CRC32_table_generated=false;

static void CRC32_gentab()
{
	for (int i = 0; i < 256; i++)
	{
		tetrabyte crc = i;
		for (int j = 8; j > 0; j--)
			if (crc & 1)
				crc = (crc >> 1) ^ CRC32POLY;
			else
				crc >>= 1;
		CRC32_table[i] = crc;
	}
	CRC32_table_generated=true;
}

tetrabyte CRC32 (byte *block, size_t length, tetrabyte in_CRC)
{
	if (CRC32_table_generated==false)
		CRC32_gentab();

	tetrabyte crc = in_CRC ^ 0xFFFFFFFF;

	for (size_t i = 0; i < length; i++)
		crc = ((crc >> 8) & 0x00FFFFFF) ^ CRC32_table[(crc ^ *block++) & 0xFF];

	return crc ^ 0xFFFFFFFF;
}

int compare_size_t(void* leftp, void* rightp)
{
	size_t left = (size_t)leftp, right = (size_t)rightp;
	if (left < right)
		return -1;
	else if (left > right)
		return 1;
	else 
	{
		oassert (left == right);
		return 0;
	};
};

int compare_int(void* leftp, void* rightp)
{
	int left = (int)leftp, right = (int)rightp;
	if (left < right)
		return -1;
	else if (left > right)
		return 1;
	else 
	{
		oassert (left == right);
		return 0;
	};
};

int compare_tetrabytes(const void* leftp, const void* rightp)
{
	tetrabyte left = (tetrabyte)leftp, right = (tetrabyte)rightp;
	if (left < right)
		return -1;
	else if (left > right)
		return 1;
	else 
	{
		oassert (left == right);
		return 0;
	};
};

bool element_in_the_array_of_tetrabytes(tetrabyte i, tetrabyte *a, unsigned size)
{
	// SIMD can be used here
	for (unsigned j=0; j<size; j++)
		if (a[j]==i)
			return true;
	return false;
};

bool element_in_the_array_of_size_t(size_t i, size_t *a, unsigned size)
{
	// SIMD can be used here
	for (unsigned j=0; j<size; j++)
		if (a[j]==i)
			return true;
	return false;
};

void add_value_to_each_element_of_size_t_array (size_t *a, size_t s, size_t val)
{
	for (size_t i=0; i<s; i++)
		a[i]+=val;
};

int qsort_compare_tetrabytes(const void* leftp, const void* rightp)
{
	tetrabyte left = *(tetrabyte*)leftp, right = *(tetrabyte*)rightp;
	if (left < right)
		return -1;
	else if (left > right)
		return 1;
	else 
	{
		oassert (left == right);
		return 0;
	};
};

void tetrabyte_array_remove_all_values(tetrabyte** array, tetrabyte val, size_t *size, bool call_drealloc)
{
	// remove all val elements from array
	size_t dst=0;
	size_t new_size=*size;
	tetrabyte *a=*array;
	for (size_t i=0; i<*size; i++)
	{
		if (a[i]==val)
			new_size--;
		else
		{
			if (dst!=i)
				a[dst]=a[i];
			dst++;
		};
	};
	*size=new_size;
	if (call_drealloc)
		*array=DREALLOC(*array, tetrabyte, new_size, "");
};

byte *hexstring_to_array_or_die (char *hexstring, size_t *out)
{
	size_t hexstring_len=strlen(hexstring);
	size_t buflen=hexstring_len/2;
	if (hexstring_len&1)
		die ("Something wrong with hexstring [%s]\n", hexstring);
	byte *rt=DMALLOC (byte, buflen, "byte");
	char tmp[3];
	tmp[2]=0;
	for (int i=0; i<hexstring_len; i+=2)
	{
		tmp[0]=hexstring[i];
		tmp[1]=hexstring[i+1];
		//printf ("tmp=%s\n", tmp);
		unsigned b;
		if (sscanf (tmp, "%x", &b)!=1)
			die ("Something wrong with string [%s]\n", tmp);
		rt[i/2]=(byte)b;
	};
	*out=buflen;
	return rt;
};

// negative values are OK
void range_update (struct my_range *s, ssize_t val)
{
	if (s->min_present==false)
	{
		s->_min=val;
		s->min_present=true;
	}
	else
		s->_min=min(s->_min, val);

	if (s->max_present==false)
	{
		s->_max=val;
		s->max_present=true;
	}
	else
		s->_max=max(s->_max, val);
};

// check if in range [begin, end). TODO: better fn name
bool REG_in_range (REG v, REG begin, REG end)
{
	if (v<begin)
		return false;
	if (v>=end)
		return false;
	return true;
};

// check if in range [begin, begin+size). TODO: better fn name
bool REG_in_range2 (REG v, REG begin, size_t size)
{
	return REG_in_range (v, begin, begin+size);
};

uint64_t uint64_log2 (uint64_t i)
{
	oassert (i!=0);
	
	return __lzcnt64(i);
};

// TODO rework
int popcnt32 (uint32_t x)
{
	int count = 0;
	for (int i = 0; i < 32; i++, x >>= 1)
		count += (int)x & 1;
	return count;
}

