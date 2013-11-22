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

#include "datatypes.h"

#if defined(__linux__) || defined(__APPLE__)
#include <string.h> // for strcasecmp()
#define stricmp strcasecmp
#endif

#ifdef  __cplusplus
extern "C" {
#endif

	char* str_trim_one_char_right (char *in);
	char* str_trim_all_lf_cr_right (char *in);
	char str_last_char (const char *s);
	char *remove_char_begin_end_if_present (char *s, char c);
#ifdef O_BITS64
	octabyte strtol_or_strtoll(const char *nptr, char **endptr, int base);
#else
	tetrabyte strtol_or_strtoll(const char *nptr, char **endptr, int base);
#endif    
	const char *bool_to_string(bool b);
	int find_string_in_array_of_strings(const char *s, const char **array, size_t array_size, 
		bool case_insensitive, bool sorted);
	const char *mon_name[12];
	bool string_is_ends_with (const char *s, const char *ending);

#ifdef  __cplusplus
}
#endif

