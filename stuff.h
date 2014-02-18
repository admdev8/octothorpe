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

#define CONCAT(x, y) x##y
#define WIDEN(x) CONCAT(L,x)

#define VAL_IN_BOUNDS_INCL(val,begin,end) ((val)>=(begin) && (val)<=(end))
#define OVERLAPPING_INTERVALS(begin1,end1,begin2,end2) (VAL_IN_BOUNDS_INCL(begin2,begin1,end1) || VAL_IN_BOUNDS_INCL(end2,begin1,end1))

	bool value_in2(unsigned v, unsigned a1, unsigned a2);
	bool value_in7(unsigned v, unsigned a1, unsigned a2, unsigned a3, unsigned a4, unsigned a5, unsigned a6, unsigned a7);
	unsigned most_significant_hex_number(octabyte x);
	_Noreturn void die (const char * fmt, ...);
	void* memdup (void *p, size_t s);
	void debugger_breakpoint();
	FILE *fopen_or_die(const char* fname, const char* mode);
	void make_REG_compact_hex (REG a, strbuf* out);
	void make_compact_list_of_REGs (REG *regs, unsigned regs_total, strbuf *out, unsigned limit);
	void regcomp_or_die (regex_t *_Restrict_ preg, const char *_Restrict_ pattern, int cflags);

	unsigned NULL_terminated_array_of_pointers_size(void **a);
	unsigned align_to_boundary(unsigned address, unsigned boundary);

	const char *find_content_type_for_filename (const char *filename);
	tetrabyte CRC32 (byte *block, size_t length, tetrabyte in_CRC);

	// used in rbtree
	int compare_size_t(void* leftp, void* rightp);
	int compare_int(void* leftp, void* rightp);
	int compare_tetrabytes(const void* leftp, const void* rightp);

	bool element_in_the_array_of_tetrabytes(tetrabyte i, tetrabyte *a, unsigned size);
	bool element_in_the_array_of_size_t(size_t i, size_t *a, unsigned size);
	void add_value_to_each_element_of_size_t_array (size_t *a, size_t s, size_t val);
	int qsort_compare_tetrabytes(const void* leftp, const void* rightp);
	void tetrabyte_array_remove_all_values(tetrabyte** array, tetrabyte val, size_t *size, bool call_drealloc);
	
#ifdef  __cplusplus
}
#endif
