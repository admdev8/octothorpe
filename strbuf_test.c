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
#include <stdio.h>
#include <limits.h>

int main()
{
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
    dump_unfreed_blocks();
    dmalloc_deinit();

    return 0;
};

/* vim: set expandtab ts=4 sw=4 : */
