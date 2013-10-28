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

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "datatypes.h"
#include <string.h>

#include "strbuf.h"
#include "regex.h"

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef OPTIONAL
#define OPTIONAL
#endif

#define SMASH(x, y) x##y
#define WIDEN(x) SMASH(L,x)

#define VAL_IN_BOUNDS_INCL(val,begin,end) ((val)>=(begin) && (val)<=(end))

	bool value_in(unsigned v, unsigned a1, unsigned a2, unsigned a3, unsigned a4, unsigned a5, unsigned a6, unsigned a7);
	unsigned most_significant_hex_number(octabyte x);
	_Noreturn void die (const char * fmt, ...);
	void* memdup (void *p, size_t s);
	void print_string_range (const char *s, int begin, size_t size);
	byte* load_file_or_die (const char* fname, size_t *fsize);
	char* str_trim_one_char_right (char *in);
	char* str_trim_all_lf_cr_right (char *in);
	char *remove_char_begin_end_if_present (char *s, char c);
	void debugger_breakpoint();
	FILE *fopen_or_die(const char* fname, const char* mode);
	int stricmp_range (const char *s1, int s1_begin, int s1_end, const char *s2);
	void make_REG_compact_hex (REG a, strbuf* out);
	void make_compact_list_of_REGs (REG *regs, unsigned regs_total, strbuf *out, unsigned limit);
	void regcomp_or_die (regex_t *_Restrict_ preg, const char *_Restrict_ pattern, int cflags);

#ifdef _WIN64
	octabyte strtol_or_strtoll(const char *nptr, char **endptr, int base);
#else
	tetrabyte strtol_or_strtoll(const char *nptr, char **endptr, int base);
#endif    
	const char *bool_to_string(bool b);

	unsigned NULL_terminated_array_of_pointers_size(void **a);
	unsigned align_to_boundary(unsigned address, unsigned boundary);

	int find_string_in_array_of_strings(const char *s, const char **array, size_t array_size, 
		bool case_insensitive, bool sorted);

	//#ifndef _MSC_VER
	//#define stricmp strcasecmp
	//#endif

#ifdef  __cplusplus
}
#endif
