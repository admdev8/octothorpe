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

#ifdef _WIN32
#include <windows.h>
#endif
#include "oassert.h"
#include <memory.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

#include "datatypes.h"
#include "logging.h"
#include "dmalloc.h"
#include "rbtree.h"
#include "strbuf.h"
#include "stuff.h"
#include "inttypes.h"
#include "fmt_utils.h"

// rationale: writting to both stdout and log-file
fds cur_fds={ NULL, NULL };

bool L_timestamp=false;
int L_ofs_width=__WORDSIZE;

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

void L_init_stdout_only ()
{
    cur_fds.fd1=stdout;
};

void L_init (const char* fname)
{
    cur_fds.fd1=stdout;

    cur_fds.fd2=fopen(fname, "w");
    if (cur_fds.fd2==NULL)
        die ("Can't create %s for writing.\n", fname);
    int i=setvbuf(cur_fds.fd2, NULL, _IONBF, 0); // no buffering
    oassert(i==0);
    atexit(L_deinit);
};

void L_va (const char * fmt, va_list va)
{
    L_fds_va(&cur_fds, fmt, va);
};

void L_fds_va (fds *s, const char * fmt, va_list va)
{
    static bool last_char_was_CR_or_unknown=true;
    // MinGW defines _WIN32
#ifdef _WIN32
    SYSTEMTIME t; // win32 only yet
#endif

    if (L_timestamp && last_char_was_CR_or_unknown)
    {
#ifdef _WIN32
        GetLocalTime (&t);
        if (s->fd1)
            fprintf (s->fd1, "[%04d-%02d-%02d %02d:%02d:%02d:%03d] ", 
                    t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
        if (s->fd2)
            fprintf (s->fd2, "[%04d-%02d-%02d %02d:%02d:%02d:%03d] ", 
                    t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
#else
       oassert(!"to be implemented!");
       fatal_error();
#endif
    };

    last_char_was_CR_or_unknown=(strlen(fmt)>0 && fmt[strlen(fmt)-1]=='\n') ? true : false;
   
    // we need this voodoo: http://stackoverflow.com/questions/15923210/is-it-possible-to-use-va-list-method-twice-in-a-function
    va_list va_copied;
    va_copy (va_copied, va);

    if (s->fd1)
        vfprintf (s->fd1, fmt, va);

    if (s->fd2)
        vfprintf (s->fd2, fmt, va_copied);
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
        once_was_printed=rbtree_create(true, "rbtree: once_was_printed", (int (*)(void*,void*))strcmp);

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

void L_print_buf_ofs_fds (fds *s, byte *buf, size_t size, size_t ofs)
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

        if (L_ofs_width==64)
            L_fds (s, PRI_OCTA_HEX_PAD ": ", starting_offset + pos + ofs);
        else if (L_ofs_width==32)
            L_fds (s, PRI_TETRA_HEX_PAD ": ", starting_offset + pos + ofs);
        else
            oassert(!"incorrect L_ofs_width");
        
        for (i=0; i<wpn; i++)
            L_fds (s, "%02X%c", buf[pos+i], (i==7) ? '-' : ' ');

        if (wpn<16)
            for (i=0; i<16-wpn; i++)
                L_fds (s, "   ");

        L_fds (s, "\"");

        for (i=0; i<wpn; i++)
            L_fds (s, "%c", isprint (buf[pos+i]) ? buf[pos+i] : '.');

        if (wpn<16)
            for (i=0; i<16-wpn; i++)
                L_fds (s, " ");

        L_fds (s, "\"\n");

        pos+=wpn;
    };
};

// in C/C++ format
void L_print_buf_ofs_fds_C (fds *s, byte *buf, size_t size, size_t ofs)
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

        if (L_ofs_width==64)
            L_fds (s, "/*" PRI_OCTA_HEX_PAD "*/ ", starting_offset + pos + ofs);
        else if (L_ofs_width==32)
            L_fds (s, "/*" PRI_TETRA_HEX_PAD "*/ ", starting_offset + pos + ofs);
        else
            oassert(!"incorrect L_ofs_width");
        
        for (i=0; i<wpn; i++)
            L_fds (s, "0x%02X%c ", buf[pos+i], (pos+i+1)==size ? ' ' : ',');

        if (wpn<16)
            for (i=0; i<16-wpn; i++)
                L_fds (s, "   ");

        L_fds (s, "/*");

        for (i=0; i<wpn; i++)
            L_fds (s, "%c", isprint (buf[pos+i]) ? buf[pos+i] : '.');

        if (wpn<16)
            for (i=0; i<16-wpn; i++)
                L_fds (s, " ");

        L_fds (s, "*/\n");

        pos+=wpn;
    };
};

void L_print_buf_ofs (byte *buf, size_t size, size_t ofs)
{
    L_print_buf_ofs_fds (&cur_fds, buf, size, ofs);
};

void L_print_buf (byte *buf, size_t size)
{
    L_print_buf_ofs (buf, size, 0);
};

// in C/C++ format
void L_print_buf_ofs_C (byte *buf, size_t size, size_t ofs)
{
    L_print_buf_ofs_fds_C (&cur_fds, buf, size, ofs);
};

void L_print_bufs_diff (byte *buf1, byte *buf2, size_t size)
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
            if (L_ofs_width==64)
                L (PRI_OCTA_HEX_PAD ": ", starting_offset + pos);
            else if (L_ofs_width==32)
                L (PRI_TETRA_HEX_PAD ": ", starting_offset + pos);
            else
                oassert(!"incorrect L_ofs_width");
            
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

/* vim: set expandtab ts=4 sw=4 : */
