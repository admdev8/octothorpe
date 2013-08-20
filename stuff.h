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

unsigned most_significant_hex_number(octabyte x);
void die (const char * fmt, ...);
void* memdup (void *p, size_t s);
void print_string_range (const char *s, int begin, size_t size);
byte* load_file_or_die (const char* fname, size_t *fsize);
char* str_trim_one_char_right (char *in);
void debugger_breakpoint();
void fill_by_tetrabytes (void* ptr, size_t size, tetrabyte val);
FILE *fopen_or_die(const char* fname, const char* mode);
int stricmp_range (const char *s1, int s1_begin, int s1_end, const char *s2);
void make_REG_compact_hex (REG a, strbuf* out);
void make_compact_list_of_REGs (REG *regs, unsigned regs_total, strbuf *out, unsigned limit);
void regcomp_or_die (regex_t *_Restrict_ preg, const char *_Restrict_ pattern, int cflags);

int strtol_or_strtoll(const char *nptr, char **endptr, int base);

//#ifndef _MSC_VER
//#define stricmp strcasecmp
//#endif

#ifdef  __cplusplus
}
#endif

/* vim: set expandtab ts=4 sw=4 : */
