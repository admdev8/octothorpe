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
#include "dmalloc.h"
#include "fmt_utils.h"

int main()
{
#if __WORDSIZE==64
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
	
	oassert (str_common_prefix_len("asd", "das")==0);
	oassert (str_common_prefix_len(" & Her Lover", " Cook, the Thief, His Wife & Her Lover")==1);
	oassert (str_common_prefix_len("asd", "as2")==2);
	oassert (str_common_prefix_len("asd", "as")==2);
	oassert (str_common_prefix_len("asd", "asd")==3);
	oassert (str_common_prefix_len("asd", "abb")==1);


	char *buf=DSTRDUP("hello world","buf");
	string_remove_part (buf, 0, 4); // "hello"
	string_remove_part (buf, 0, 0); // " "
	oassert (strcmp (buf, "world")==0);
	DFREE(buf);

	buf=DSTRDUP(" ","buf");
	string_remove_part (buf, 0, 0); // " "
	oassert (strcmp (buf, "")==0);
	DFREE(buf);

	buf=DSTRDUP("asdfg","buf");
	string_remove_part (buf, 4, 4);
	string_remove_part (buf, 0, 0);
	oassert (strcmp (buf, "sdf")==0);
	DFREE(buf);

	oassert(is_string_consists_only_of_Latin_letters("abcdef")==true);
	oassert(is_string_consists_only_of_Latin_letters("abcd1ef")==false);
	oassert(is_string_has_only_one_character_repeating("111111111", NULL)==true);
	oassert(is_string_has_only_one_character_repeating("1111111112", NULL)==false);
	
	char* tmp=dmalloc_and_snprintf ("%d %d", 123, 456);
	oassert(streq(tmp, "123 456"));
	DFREE(tmp);
	
	tmp=dmalloc_and_snprintf ("%" PRIx64 " %d", 0x1234567890ABCDEF, 456);
	oassert(streq(tmp, "1234567890abcdef 456"));
	DFREE(tmp);
};

