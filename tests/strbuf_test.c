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

test_addc_and_addf()
{
	strbuf s=STRBUF_INIT;
	strbuf_addstr (&s, "hello");
	strbuf_addc (&s, ' ');
	strbuf_addf (&s, "w%crld", 'o');
	strbuf_puts (&s);
	strbuf_deinit (&s);
};

char **my_environ;

void test_env_vars_expansion()
{
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
	oassert (strcmp(s.buf, "hello!0x123412341234")==0);
	strbuf_deinit (&s);
#endif

	test_addc_and_addf();

	dump_unfreed_blocks();
	dmalloc_deinit();

	return 0;
};
