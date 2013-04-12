#include "strbuf.h"
#include "dmalloc.h"
#include <stdio.h>

void main()
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
    
    strbuf_deinit(&s);
    dump_unfreed_blocks();
    dmalloc_deinit();
};
