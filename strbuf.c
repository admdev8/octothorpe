#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "strbuf.h"
#include "dmalloc.h"
#include "stuff.h"

char* strbuf_dummybuf="\x00";

void strbuf_init (strbuf *sb, size_t size)
{
    // beware: sb->buf may contain some garbage like 0xcccccccc
#if 0
    if (sb->buf && sb->buf!=strbuf_dummybuf)
    {
        //DFREE (sb->buf); // do not do this. existing strbuf shouldn't be reinitialized!
        assert(!"strbuf is already have something");
    };
#endif
    sb->buf=DMALLOC(char, size, "strbuf");
    if (size>0)
        sb->buf[0]=0;
    sb->strlen=0;
    sb->buflen=size;
};

void strbuf_deinit(strbuf *sb)
{
    if (sb->buf && sb->buf!=strbuf_dummybuf)
        DFREE(sb->buf);
};

void strbuf_reinit(strbuf *sb, size_t size)
{
    strbuf_deinit (sb);
    strbuf_init (sb, size);
};

void strbuf_grow (strbuf *sb, size_t size)
{
    char* new_buf;

    //printf (__func__"() size=%d\n", size);

    if (size < (sb->buflen - sb->strlen))
    {
        //printf ("(no need to reallocate)\n");
        return; // we have space already
    };

    new_buf=DMALLOC(char, sb->strlen + size + 1, "strbuf"); // FIXME: realloc or DREALLOC should be here for clarity
    if (sb->buf)
    {
        memcpy (new_buf, sb->buf, sb->strlen+1);
        if (sb->buf!=strbuf_dummybuf)
            DFREE(sb->buf);
    };
    sb->buf=new_buf;
    sb->buflen=sb->strlen + size + 1;
};

void strbuf_addstr_range (strbuf *sb, const char *s, int len)
{
    // FIXME: rework this function using strbuf_addstr_range_be()
    strbuf_grow (sb, len+1);
    memcpy (sb->buf + sb->strlen, s, len);
    sb->strlen+=len;
    sb->buf[sb->strlen]=0;
};

void strbuf_addstr (strbuf *sb, const char *s)
{
    strbuf_addstr_range (sb, s, strlen(s));
};

void strbuf_addstr_range_be (strbuf *sb, const char *s, unsigned begin, unsigned end)
{
    int i;

    assert (begin<end);
   
    // FIXME: to be optimized
 
    for (i=begin; i<end; i++)
        strbuf_addc (sb, s[i]); 
};

void strbuf_addc (strbuf *sb, char c)
{
    strbuf_grow(sb, 1);
    sb->buf[sb->strlen]=c;
    sb->strlen++;
    sb->buf[sb->strlen]=0;
};

bool strbuf_replace_if_possible (strbuf *sb, const char *s1, const char *s2)
{
    char *t=strstr (sb->buf, s1);
    char *newbuf;
    unsigned newbuf_cursize=0, newbuf_newsize;

    if (t==NULL)
        return false;
    
    // now rebuild buf

    newbuf_newsize=sb->strlen-strlen(s1)+strlen(s2)+1;
    newbuf=DMALLOC(char, newbuf_newsize, "strbuf::buf"); // C++ style naming? haha

    // 1st part
    newbuf_cursize=t-sb->buf;
    memcpy (newbuf, sb->buf, newbuf_cursize);

    // 2nd part
    memcpy (newbuf+newbuf_cursize, s2, strlen(s2));
    newbuf_cursize+=strlen(s2);

    // 3rd part
    memcpy (newbuf+newbuf_cursize, t+strlen(s1), newbuf_newsize-newbuf_cursize);
    newbuf[newbuf_newsize-1]=0;
    
    DFREE (sb->buf);
    sb->buf=newbuf;
    sb->buflen=newbuf_newsize;
    sb->strlen=newbuf_newsize-1;

    return true;
};

void strbuf_vaddf (strbuf *sb, const char *fmt, va_list va)
{
   size_t sz=_vscprintf (fmt, va); // MSVC-specific?

   strbuf_grow(sb, sz);

#ifdef _MSC_VER   
   if (vsnprintf_s (sb->buf + sb->strlen, sz+1, sz, fmt, va)==-1) // MSVC-specific
#else
   if (vsnprintf (sb->buf + sb->strlen, sz+1, fmt, va)==-1)
#endif
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

void make_uint32_compact (tetrabyte a, strbuf* out)
{
    if (a<10)
        strbuf_addf(out, "%d", a);
    else
        strbuf_addf(out, "0x%x", a);
};

void make_uint64_compact (octabyte a, strbuf* out)
{
    if (a<10)
        strbuf_addf (out, "%I64lld", a);
    else
        strbuf_addf (out, "0x%I64llx", a);
};

void make_SIZE_T_compact (size_t a, strbuf* out)
{
    if (sizeof(size_t)==sizeof(octabyte))
        make_uint64_compact (a, out);
    else if (sizeof(size_t)==sizeof(tetrabyte))
        make_uint32_compact (a, out);
    else
    {
        assert (0);
    };
};

void strbuf_asmhex(strbuf *out, octabyte v)
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

void strbuf_addc_C_escaped (strbuf *s, char c, bool treat_any_as_binary)
{
    if (treat_any_as_binary)
        strbuf_addf (s, "\\x%02X", c);
    else
    {
        switch (c)
        {
            case '\n':
                strbuf_addstr (s, "\\n");
                break;
            case '\r':
                strbuf_addstr (s, "\\r");
                break;
            case '"':
                strbuf_addstr (s, "\\\"");
                break;
            case '\\':
                strbuf_addstr (s, "\\\\");
                break;
            default:
                if (c>0 && c<0x20)
                    strbuf_addf (s, "\\x%02X", c);
                else
                    strbuf_addc (s, c);
                break;
        };
    };
};

void strbuf_cvt_to_C_string (strbuf *s, strbuf *out, bool treat_as_binary)
{
    size_t i;

    for (i=0; i<s->strlen; i++)
        strbuf_addc_C_escaped (out, s->buf[i], treat_as_binary);
};

// replace %substring% to environment variable, if possible
void env_vars_expansion(strbuf *sb)
{
    int i=0;
    while (environ[i])
    {
        char *s=DSTRDUP (environ[i], "env");
        char *s1=strtok (s, "=");
        char *s2=strtok (NULL, "=");
        strbuf percented_env_var=STRBUF_INIT;
        strbuf_addc (&percented_env_var, '%');
        strbuf_addstr (&percented_env_var, s1);
        strbuf_addc (&percented_env_var, '%');

        strbuf_replace_if_possible (sb, percented_env_var.buf, s2);

        strbuf_deinit(&percented_env_var);
        DFREE(s);
        i++;
    };
};

char *strbuf_detach(strbuf *s, size_t *out_size)
{
    char *rt=s->buf;

    if (out_size)
        *out_size=s->strlen;

    strbuf_init(s, 0); // don't call _reinit here

    return rt;
};

char strbuf_last_char (strbuf *s)
{
    assert (s->strlen!=0);
    return s->buf[s->strlen-1];
};
