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

#include "datatypes.h"
#include <stdbool.h>

#ifdef  __cplusplus
extern "C" {
#endif
	
	bool streq (char *s1, char *s2);
	bool strieq (char *s1, char *s2);
	char* str_trim_one_char_right (char *in);
	char* str_trim_all_lf_cr_right (char *in);
	char str_last_char (const char *s);
	char *remove_char_begin_end_if_present (char *s, char c);
	size_t strtol_or_strtoll(const char *nptr, char **endptr, int base);
	const char *bool_to_string(bool b);
	int find_string_in_array_of_strings(const char *s, const char **array, size_t array_size, 
		bool case_insensitive, bool sorted);
	const char *mon_name[12];
	bool string_is_ends_with (const char *s, const char *ending);
	unsigned str_common_prefix_len (const char *s1, const char *s2);
	byte* cvt_to_widestr_and_allocate (char *str, size_t *len);
	void string_remove_part (char *buf, int begin, int end);

	// -> "aaaaaa ...(123 skipped)...bbbbbb"
	void fprint_shrinked_string (char *s, size_t limit, FILE *f);
	char* dmalloc_and_snprintf (const char *fmt, ...);

	bool is_string_consists_only_of_Latin_letters (char *s);
	bool is_string_consists_only_of_hex_digits (char *s);
	bool is_string_has_only_one_character_repeating (char *s);

#ifdef  __cplusplus
}
#endif

