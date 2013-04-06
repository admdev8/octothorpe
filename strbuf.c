#include "strbuf.h"
#include "dmalloc.h"
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include "stuff.h"

void strbuf_init (strbuf *sb, size_t size)
{
    sb->buf=(char*)DMALLOC(size, "strbuf");
    sb->buf[0]=0;
    sb->strlen=0;
    sb->buflen=size;
};

void strbuf_deinit(strbuf *sb)
{
    DFREE(sb->buf);
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
    if (sb->buf)
    {
        memcpy (new_buf, sb->buf, sb->strlen+1);
        DFREE(sb->buf);
    };
    sb->buf=new_buf;
    sb->buflen=sb->strlen + size + 1;
};

void strbuf_addstr_range (strbuf *sb, const char *s, int len)
{
    strbuf_grow (sb, len+1);
    memcpy (sb->buf + sb->strlen, s, len);
    sb->strlen+=len;
    sb->buf[sb->strlen]=0;
};

void strbuf_addstr (strbuf *sb, const char *s)
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

void make_uint32_compact (uint32_t a, strbuf* out)
{
    if (a<10)
        strbuf_addf(out, "%d", a);
    else
        strbuf_addf(out, "0x%x", a);
};

void make_uint64_compact (uint64_t a, strbuf* out)
{
    if (a<10)
        strbuf_addf (out, "%I64lld", a);
    else
        strbuf_addf (out, "0x%I64llx", a);
};

void make_SIZE_T_compact (size_t a, strbuf* out)
{
    if (sizeof(size_t)==sizeof(uint64_t))
        make_uint64_compact (a, out);
    else if (sizeof(size_t)==sizeof(uint32_t))
        make_uint32_compact (a, out);
    else
    {
        assert (0);
    };
};

void strbuf_asmhex(strbuf *out, uint64_t v)
{
    if (v<10)
        strbuf_addf(out, "%d", v);
    else
        if (most_significant_hex_number(v)<=9)
            strbuf_addf (out, "%llXh", v);
        else
            strbuf_addf (out, "0%llXh", v);
};

void strbuf_puts (strbuf *sb)
{
    puts (sb->buf);
};

