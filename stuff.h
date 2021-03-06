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

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "datatypes.h"
#include "strbuf.h"
#include "regex.h"

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef __GNUC__
#define MY_NORETURN _Noreturn
#elif _MSC_VER
#define MY_NORETURN __declspec(noreturn)
#else
#define MY_NORETURN
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

// FIXME get rid of these!
#define VAL_IN_BOUNDS_INCL(val,begin,end) ((val)>=(begin) && (val)<=(end))
#define OVERLAPPING_INTERVALS(begin1,end1,begin2,end2) (VAL_IN_BOUNDS_INCL(begin2,begin1,end1) || VAL_IN_BOUNDS_INCL(end2,begin1,end1))

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#define _1KiB 1024
#define _1MiB (1024*1024)
#define _1GiB (1024*1024*1024)

// can be initialized with bzero
	struct my_range
	{
		ssize_t _min;
		bool min_present;

		ssize_t _max;
		bool max_present;
	};

	bool value_in2(unsigned v, unsigned a1, unsigned a2);
	bool value_not_in2(unsigned v, unsigned a1, unsigned a2);
	bool value_in7(unsigned v, unsigned a1, unsigned a2, unsigned a3, unsigned a4, unsigned a5, unsigned a6, unsigned a7);
	unsigned most_significant_hex_number(octa x);
	MY_NORETURN void die (const char * fmt, ...);
	void* memdup (void *p, size_t s);
	void debugger_breakpoint();
	void make_REG_compact_hex (REG a, strbuf* out);
	void make_compact_list_of_REGs (REG *regs, unsigned regs_total, strbuf *out, unsigned limit);

	// all functions works with NULL-terminated arrays
	unsigned NULL_terminated_array_of_pointers_size(void **a);
	void print_array_of_strings (char **s);
	void dfree_array_of_blocks (char **s);
	
	unsigned align_to_boundary(unsigned address, unsigned boundary);

	const char *find_content_type_for_filename (const char *filename);
	tetra CRC32 (byte *block, size_t length, tetra in_CRC);
	octa CRC64(octa crc, byte *buf, size_t len);

	// used in rbtree
	int compare_size_t(void* leftp, void* rightp);
	int compare_int(void* leftp, void* rightp);
	int compare_tetras(const void* leftp, const void* rightp);

	bool element_in_the_array_of_tetras(tetra i, tetra *a, unsigned size);
	bool element_in_the_array_of_size_t(size_t i, size_t *a, unsigned size);
	int find_element_in_the_array_of_size_t(size_t i, size_t *a, unsigned size);
	void add_value_to_each_element_of_size_t_array (size_t *a, size_t s, size_t val);
	int qsort_compare_tetras(const void* leftp, const void* rightp);
	void tetra_array_remove_all_values(tetra** array, tetra val, size_t *size, bool call_drealloc);
	byte *hexstring_to_array_or_die (char *hexstring, size_t *out);

// check if in range [begin, end). TODO: better fn name
	bool REG_in_range (REG v, REG begin, REG end);
// check if in range [begin, begin+size). TODO: better fn name
	bool REG_in_range2 (REG v, REG begin, size_t size);

	int popcnt32 (tetra x);
	int popcnt64 (octa x);

	wyde swap_endianness16 (wyde a);
	tetra swap_endianness32 (tetra a);

	bool AND_array_of_bools(bool* array, size_t size);

#ifdef  __cplusplus
}
#endif
