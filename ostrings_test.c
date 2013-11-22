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

#include <stdio.h>
#include <stdbool.h>
#include "oassert.h"
#include "ostrings.h"

int main()
{
#ifdef O_BITS64
	oassert(strtol_or_strtoll("0xAB12345678", NULL, 16)==0xAB12345678);
#else
	oassert(strtol_or_strtoll("0x12345678", NULL, 16)==0x12345678);
#endif

	const char* strings[]={"string1", "hello", "world", "another string"};
	size_t strings_t=sizeof(strings)/sizeof(char*);

	printf ("%d\n", find_string_in_array_of_strings("another string", strings, strings_t, false, false));
	printf ("%d\n", find_string_in_array_of_strings("world", strings, strings_t, true, false));
	printf ("%d\n", find_string_in_array_of_strings("Hello", strings, strings_t, true, false));
	printf ("%d\n", find_string_in_array_of_strings("world2", strings, strings_t, true, false));
	printf ("%d\n", find_string_in_array_of_strings("World", strings, strings_t, false, false));

	oassert (string_is_ends_with ("hello", "lo")==true);
	oassert (string_is_ends_with ("hello", "lo1")==false);
};
