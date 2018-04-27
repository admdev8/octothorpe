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

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdio.h>
#include "datatypes.h"
#include "stuff.h"

	// TODO: wydefill()
	void bytefill (void* ptr, size_t size, byte val);
	void tetrafill (void* ptr, size_t size, tetra val);

#ifndef bzero
	void bzero (void* ptr, size_t s);
#endif
	
	bool is_blk_zero (void *ptr, size_t s);
	byte *omemmem (byte *haystack, size_t haystack_size, byte *needle, size_t needle_size);
	byte *kmp_search(byte *haystack, size_t haystack_size, byte *needle, size_t needle_size);
	size_t* find_all_needles (byte *haystack, size_t haystack_size, byte* needle, size_t needle_size, 
		OUT size_t* rt_size);
	size_t omemmem_count (byte *haystack, size_t haystack_size, byte *needle, size_t needle_size);
	void XOR_block (byte* a, byte* b, size_t s);
	bool is_buf_printable (char *s, size_t size);
	// -1 if not found
	int search_for_elem_in_array_of_size_t (size_t* array, size_t size, size_t needle);

#ifdef  __cplusplus
}
#endif
