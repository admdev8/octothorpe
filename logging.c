#include <assert.h>
#include <memory.h>
#include <stdlib.h>

#include "logging.h"
#include "dmalloc.h"
#include "rbtree.h"
#include "strbuf.h"
#include "stuff.h"

// rationale: writting to both stdout and log-file
fds cur_fds={ NULL, NULL };

bool L_timestamp=false;
bool L_quiet=false;

// set actually is here
rbtree *once_was_printed=NULL;

void L_deinit(void)
{
    if (cur_fds.fd2!=NULL)
    {
        fclose(cur_fds.fd2);
        cur_fds.fd2=NULL;
    };

    if (once_was_printed)
    {
        rbtree_foreach(once_was_printed, NULL, dfree, NULL);
        rbtree_deinit(once_was_printed);
        once_was_printed=NULL;
    };
};

void L_init (const char* fname)
{
    int i;

    cur_fds.fd1=stdout;

    cur_fds.fd2=fopen(fname, "w");
    if (cur_fds.fd2==NULL)
        die ("Can't create %s for writing.\n", fname);
    i=setvbuf(cur_fds.fd2, NULL, _IONBF, 0);
    assert(i==0);
    atexit(L_deinit);
};

void L_va (const char * fmt, va_list va)
{
    L_fds_va(&cur_fds, fmt, va);
};

void L_fds_va (fds *s, const char * fmt, va_list va)
{
#ifdef _MSC_VER
    SYSTEMTIME t; // win32 only yet
#endif

    if (L_quiet)
        return;

    if (L_timestamp)
    {
#ifdef _MSC_VER
        GetLocalTime (&t);
        if (s->fd1)
            fprintf (s->fd1, "[%04d-%02d-%02d %02d:%02d:%02d:%03d]", 
                    t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
        if (s->fd2)
            fprintf (s->fd2, "[%04d-%02d-%02d %02d:%02d:%02d:%03d]", 
                    t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
#else
       assert(!"to be implemented!");
#endif
    };

    if (s->fd1)
        vfprintf (s->fd1, fmt, va);
    if (s->fd2)
        vfprintf (s->fd2, fmt, va);
};

void L (const char * fmt, ...)
{
    va_list va;
    va_start (va, fmt);

    L_va (fmt, va);
};

void L_fds (fds *s, const char * fmt, ...)
{
    va_list va;
    va_start (va, fmt);

    L_fds_va (s, fmt, va);
};

void L_fds_strbuf (fds *s, strbuf *sb)
{
    L_fds (s, "%s", sb->buf);
};

void L_once_va (const char * fmt, va_list va)
{
    strbuf sb=STRBUF_INIT;
    void *found;
    char *s;

    if (once_was_printed==NULL)
        once_was_printed=rbtree_create(true, "rbtree: once_was_printed", strcmp);

    strbuf_vaddf(&sb, fmt, va);
    
    //printf (__func__"() sb=");
    //strbuf_puts (&sb);
    
    found=rbtree_lookup(once_was_printed, sb.buf);
    //printf ("found=0x%p\n", found);

    if (found)
    {
        strbuf_deinit(&sb);
        return;
    };

    L_va (fmt, va);

    s=DSTRDUP(sb.buf, "L_once_va()");
    //printf ("s=%s\n", s);
    rbtree_insert (once_was_printed, s, "FIXME"); // here will be 'set' someday
    strbuf_deinit(&sb);
};

void L_once (const char * fmt, ...)
{
    va_list va;
    va_start (va, fmt);

    L_once_va (fmt, va);
};

void L_print_buf_ofs (uint8_t *buf, size_t size, size_t ofs)
{
    size_t pos=0;
    unsigned starting_offset=0;
    unsigned i;

    while (size-pos)
    {
        size_t wpn;
        if ((size-pos)>16)
            wpn=16;
        else
            wpn=size-pos;

#ifdef _WIN64
        L ("%016I64x: ", starting_offset + pos + ofs);
#else
        L ("%08X: ", starting_offset + pos + ofs);
#endif
        for (i=0; i<wpn; i++)
            L ("%02X%c", buf[pos+i], (i==7) ? '-' : ' ');

        if (wpn<16)
            for (i=0; i<16-wpn; i++)
                L ("   ");

        L ("\"");

        for (i=0; i<wpn; i++)
            L ("%c", isprint (buf[pos+i]) ? buf[pos+i] : '.');

        if (wpn<16)
            for (i=0; i<16-wpn; i++)
                L (" ");

        L ("\"\n");

        pos+=wpn;
    };
};

void L_print_buf (uint8_t *buf, size_t size)
{
    L_print_buf_ofs (buf, size, 0);
};

void L_print_bufs_diff (uint8_t *buf1, uint8_t *buf2, size_t size)
{
    size_t pos=0;
    unsigned starting_offset=0;
    unsigned i;
    bool dots_printed=false;

    while (size-pos)
    {
        size_t wpn;
        if ((size-pos)>16)
            wpn=16;
        else
            wpn=size-pos;

        if (memcmp (buf1+pos, buf2+pos, wpn)!=0) // any changes in the whole line?
        {
#ifdef _WIN64
            L ("%016I64x: ", starting_offset + pos);
#else
            L ("%08X: ", starting_offset + pos);
#endif
            for (i=0; i<wpn; i++)
            {
                if (buf1[pos+i]!=buf2[pos+i])
                    L ("%02X", buf2[pos+i]);
                else
                    L ("  ");
                
                L ("%c", (i==7) ? '-' : ' ');
            };

            if (wpn<16)
                for (i=0; i<16-wpn; i++)
                    L ("   ");

            L ("\"");

            for (i=0; i<wpn; i++)
            {
                if (buf1[pos+i]!=buf2[pos+i])
                    L ("%c", isprint (buf2[pos+i]) ? buf2[pos+i] : '.');
                else
                    L (" ");
            };

            if (wpn<16)
                for (i=0; i<16-wpn; i++)
                    L (" ");

            L ("\"\n");
            dots_printed=false;
        }
        else
        {
            if (dots_printed==false)
            {
                L (" ... \n");
                dots_printed=true;
            };
        };
        pos+=wpn;
    };
};

