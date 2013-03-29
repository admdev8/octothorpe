#include "strbuf.h"
#include "dmalloc.h"
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

void strbuf_init (strbuf *sb, size_t size)
{
    sb->buf=(char*)DMALLOC(size, "strbuf");
    sb->buf[0]=0;
    sb->strlen=0;
    sb->buflen=size;
};

void strbuf_deinit(strbuf *sb)
{
    dfree(sb->buf);
};

void strbuf_grow (strbuf *sb, size_t size)
{
    char* new_buf;

    //printf (__FUNCTION__"() size=%d\n", size);

    if (size < (sb->buflen - sb->strlen))
    {
        //printf ("(no need to reallocate)\n");
        return; // we have space already
    };

    new_buf=(char*)DMALLOC(sb->strlen + size + 1, "strbuf"); // FIXME: realloc or DREALLOC should be here for clarity
    memcpy (new_buf, sb->buf, sb->strlen+1);
    dfree(sb->buf);
    sb->buf=new_buf;
    sb->buflen=sb->strlen + size + 1;
};

void strbuf_addstr_range (strbuf *sb, char *s, int len)
{
    strbuf_grow (sb, len+1);
    memcpy (sb->buf + sb->strlen, s, len);
    sb->strlen+=len;
    sb->buf[sb->strlen]=0;
};

void strbuf_addstr (strbuf *sb, char *s)
{
    strbuf_addstr_range (sb, s, strlen(s));
};

void strbuf_addc (strbuf *sb, char c)
{
    strbuf_grow(sb, 1);
    sb->buf[sb->strlen]=c;
    sb->strlen++;
    sb->buf[sb->strlen]=0;
};

void strbuf_vaddf (strbuf *sb, const char *fmt, va_list va)
{
   size_t sz=_vscprintf (fmt, va); // MSVC-specific

   strbuf_grow(sb, sz);
   
   if (vsnprintf_s (sb->buf + sb->strlen, sz+1, sz, fmt, va)==-1) // MSVC-specific
   {
       assert(0);
   };
   sb->strlen+=sz;
};

void strbuf_addf (strbuf *sb, const char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	strbuf_vaddf(sb, fmt, va);
	va_end(va);
};

#ifdef TEST

void main()
{
    strbuf s;
    strbuf_init (&s, 3);
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
    strbuf_deinit(&s);
    dump_unfreed_blocks();
    dmalloc_deinit();
};

#endif
