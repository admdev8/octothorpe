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

#include "strbuf.h"
#include "dmalloc.h"
#include "oassert.h"
#include "fmt_utils.h"
#include <stdio.h>
#include <limits.h>

#ifndef LONG_LONG_MAX
#define LONG_LONG_MAX 9223372036854775807LL
#endif

void test_addc_and_addf()
{
	strbuf s=STRBUF_INIT;
	strbuf_addstr (&s, "hello");
	strbuf_addc (&s, ' ');
	strbuf_addf (&s, "w%crld", 'o');
	strbuf_puts (&s);
	strbuf_deinit (&s);
};

void test_neat_list_of_bytes()
{
	strbuf s3=STRBUF_INIT;
	byte test1[]={0,1,2,3, 0xa, 0x10, 0x33,0x34,0x35,0x36,0x37, 0x39, 0x99,0x9a};
	neat_list_of_bytes (test1, 14, &s3);
	//strbuf_puts (&s3);
	oassert (strcmp(s3.buf, "0x0 ... 0x3, 0xa, 0x10, '3' ... '7', '9', 0x99, 0x9a")==0);
	strbuf_deinit (&s3);
	
	strbuf s4=STRBUF_INIT;
	byte test2[]={0xaa, 0,1,2,3, 0xa, 0x10, 0x33,0x34,0x35,0x36,0x37, 0x39, 0x99};
	neat_list_of_bytes (test2, 14, &s4);
	//strbuf_puts (&s4);
	oassert (strcmp(s4.buf, "0xaa, 0x0 ... 0x3, 0xa, 0x10, '3' ... '7', '9', 0x99")==0);
	strbuf_deinit (&s4);
	
	strbuf s5=STRBUF_INIT;
	byte test3[]={0xaa, 0,1,2,3, 0xa, 0x10, 0x33,0x34,0x35,0x36,0x37};
	neat_list_of_bytes (test2, 12, &s5);
	//strbuf_puts (&s5);
	oassert (strcmp(s5.buf, "0xaa, 0x0 ... 0x3, 0xa, 0x10, '3' ... '7'")==0);
	strbuf_deinit (&s5);
	
	strbuf s6=STRBUF_INIT;
	byte test4[]={0xaa};
	neat_list_of_bytes (test4, 1, &s6);
	//strbuf_puts (&s6);
	oassert (strcmp(s6.buf, "0xaa")==0);
	strbuf_deinit (&s6);
	
	strbuf s7=STRBUF_INIT;
	byte test5[]={0xaa, 1,2, 0xdd};
	neat_list_of_bytes (test5, 4, &s7);
	//strbuf_puts (&s7);
	oassert (strcmp(s7.buf, "0xaa, 1, 2, 0xdd")==0);
	strbuf_deinit (&s7);
	
	strbuf s8=STRBUF_INIT;
	byte test6[]={'0', '1', '2', '3', '4', '5'};
	neat_list_of_bytes (test6, 6, &s8);
	//strbuf_puts (&s8);
	oassert (strcmp(s8.buf, "'0' ... '5'")==0);
	strbuf_deinit (&s8);
};

char **my_environ;

// FIXME: MinGW only!
void test_env_vars_expansion()
{
#if 0
	strbuf s=STRBUF_INIT;
	size_t init_len;

	strbuf_addstr (&s, "hello %TMP% world");
	init_len=s.strlen;
	env_vars_expansion(&s, my_environ);

	//strbuf_puts (&s);
	if (s.strlen==init_len) // still?
	{
		printf ("%s() failed\n", __func__);
		exit(1);
	}
	
	strbuf_deinit (&s);
#endif
};

int main(int argc, char* argv[], char* envp[])
{
	my_environ=envp;

	test_env_vars_expansion();

	//strbuf s;
	//strbuf_init (&s, 3);
	strbuf s=STRBUF_INIT;
	strbuf s2=STRBUF_INIT;
	printf ("[%s]\n", s.buf);
	printf ("sb->strlen=%d sb->buflen=%d\n", s.strlen, s.buflen);
	strbuf_addstr (&s, "string1");
	printf ("[%s]\n", s.buf);
	printf ("sb->strlen=%d sb->buflen=%d\n", s.strlen, s.buflen);
	strbuf_addc(&s, ',');
	printf ("[%s]\n", s.buf);
	printf ("sb->strlen=%d sb->buflen=%d\n", s.strlen, s.buflen);
	strbuf_addstr (&s, "string2");
	printf ("[%s]\n", s.buf);
	printf ("sb->strlen=%d sb->buflen=%d\n", s.strlen, s.buflen);
	strbuf_addc(&s, ',');
	printf ("[%s]\n", s.buf);
	printf ("sb->strlen=%d sb->buflen=%d\n", s.strlen, s.buflen);
	strbuf_addf(&s, "%d %d %d %s", 123, 456, 789, "hello");
	printf ("[%s]\n", s.buf);
	printf ("sb->strlen=%d sb->buflen=%d\n", s.strlen, s.buflen);

	strbuf_init (&s2, 0);
	strbuf_addstr (&s2, "three words here");
	strbuf_replace_if_possible (&s2, "words", "what");
	strbuf_puts (&s2);
	strbuf_deinit (&s2);

	strbuf_init (&s2, 0);
	strbuf_addstr (&s2, "three words here");
	strbuf_replace_if_possible (&s2, "three", "two");
	strbuf_puts (&s2);
	strbuf_deinit (&s2);

	strbuf_init (&s2, 0);
	strbuf_addstr (&s2, "three words here");
	strbuf_replace_if_possible (&s2, "here", "not here");
	strbuf_puts (&s2);
	strbuf_deinit (&s2);

	strbuf_init (&s2, 0);
	strbuf_addf(&s2, "%d", 12345);
	strbuf_addc(&s2, ' ');
	strbuf_addf(&s2, "%d", 12345);
	strbuf_addc(&s2, ' ');
	strbuf_addf(&s2, "%d", 12345);
	strbuf_puts (&s2);
	strbuf_deinit (&s2);

	strbuf_init (&s2, 0);
	strbuf_asmhex(&s2, LONG_LONG_MAX);
	strbuf_addc(&s2, ' ');
	strbuf_asmhex(&s2, LONG_LONG_MAX);
	strbuf_addc(&s2, ' ');
	strbuf_asmhex(&s2, LONG_LONG_MAX);
	strbuf_puts (&s2);
	strbuf_deinit (&s2);

	strbuf_deinit(&s);

	strbuf_init(&s, 0);
	strbuf_addstr (&s, "test1\r\n");
	strbuf_trim_chars_at_end (&s, "\n\r");
	oassert (strcmp(s.buf, "test1")==0);
	strbuf_deinit(&s);

	strbuf_init(&s, 0);
	strbuf_addstr (&s, "begin test test test test test test test test test test test test end");
	strbuf_fprint_short (&s, 50, stdout);
	printf ("\n");
	strbuf_deinit(&s);
/*	
	strbuf_init (&s, 0);
	strbuf_fancy_size (&s, 1200000000000);
	strbuf_puts (&s);
	printf ("\n");
	strbuf_deinit (&s);
*/	
	strbuf_init (&s, 0);
	strbuf_fancy_size (&s, 1200000000);
	oassert (strcmp(s.buf, "1.2GB")==0);
	strbuf_deinit (&s);
	
	strbuf_init (&s, 0);
	strbuf_fancy_size (&s, 1200000);
	oassert (strcmp(s.buf, "1.2MB")==0);
	strbuf_deinit (&s);
	
	strbuf_init (&s, 0);
	strbuf_fancy_size (&s, 1200);
	oassert (strcmp(s.buf, "1.2KB")==0);
	strbuf_deinit (&s);

#ifdef O_BITS64
	strbuf_init (&s, 0);
	strbuf_addf (&s, "%s!0x" PRI_ADR_HEX, "hello", 0x123412341234);
	if (strcmp(s.buf, "hello!0x123412341234")!=0)
		die ("s.buf=%s, but we need something else\n", s.buf);
	strbuf_deinit (&s);
#endif

	test_addc_and_addf();


	test_neat_list_of_bytes();

	dump_unfreed_blocks();
	dmalloc_deinit();

	return 0;
};
