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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <ctype.h>

#include "datatypes.h"
#include "stuff.h"
#include "fmt_utils.h"

#ifdef _MSC_VER
#include <intrin.h>
#endif

unsigned most_significant_hex_number(octabyte x)
{
    octabyte t=x;
    int i;

    // _BitScanReverse64 can be used here, probably (?)

    for (i=0; i<(64/4); i++)
    {
        if (t&0xF000000000000000)
        {
            //printf (__func__"(0x%016llX) -> 0x%X\n", x, t>>(64-4));
            return t>>(64-4);
        }
        else
            t=t<<4;
    };
    //printf (__func__"(0x%016llX) -> 0\n", x);
    return 0;
};

void die (const char * fmt, ...)
{
    va_list va;
    va_start (va, fmt);

    vprintf (fmt, va);
    exit(0);
};

void* memdup (void *p, size_t s)
{
    void *rt=malloc (s);
    assert(rt!=NULL);
    memcpy(rt, p, s);
    return rt;
};

void print_string_range (const char *s, int b, int e)
{
    int i;
    for (i=b; i<e; i++)
        putc(s[i], stdout);
};

byte* load_file_or_die (const char* fname, size_t *fsize)
{
    byte* rt;
    FILE* f;

    f=fopen (fname, "rb");
    if (f==NULL)
        die ("Cannot open file %s\n", fname);

    if (fseek (f, 0, SEEK_END)!=0)
        die ("fseek()\n");

    *fsize=ftell (f);
    //printf ("*fsize=%d\n", *fsize);
    rt=(byte*)malloc (*fsize);

    if (fseek (f, 0, SEEK_SET)!=0)
        die ("fseek()\n");

    if (fread (rt, *fsize, 1, f)!=1)
        die ("Cannot read file %s\n", fname);

    fclose (f);
    return rt;
};

char* str_trim_one_char_right (char *in)
{
    if (strlen(in)==0)
        return in;

    in[strlen(in)-1]=0;
    return in;
};

void debugger_breakpoint()
{
#ifdef _MSC_VER
    __debugbreak();
#else
   __asm__("int $3");
#endif
};

void fill_by_tetrabytes (void* ptr, size_t size, tetrabyte val)
{
    byte *cur_ptr=(byte*)ptr;
    size_t rem=size;

    while (rem>=4)
    {
        *(tetrabyte*)cur_ptr=val;
        cur_ptr+=sizeof(tetrabyte);
        rem-=sizeof(tetrabyte);
    };

    if (rem>=1)
        *cur_ptr=val&0xFF;
    if (rem>=2)
        *(cur_ptr+1)=(val&0xFF00)>>8;
    if (rem==3)
        *(cur_ptr+2)=(val&0xFF0000)>>16;
};

FILE *fopen_or_die(const char* fname, const char* mode)
{
    FILE *rt=fopen (fname, mode);
    if (rt==NULL)
        die ("%s(): Can't open %s file in mode '%s'\n", __func__, fname, mode);
    return rt;
};

int stricmp_range (const char *s1, int s1_begin, int s1_end, const char *s2)
{
    for (int i=s1_begin; i<s1_end; i++)
    {
        char c1=tolower(s1[i]), c2=tolower(s2[i]);
        if (c1!=c2)
            return c1-c2;
    };
    return 0;
};

void make_REG_compact_hex (REG a, strbuf* out)
{
    if (a<10)
        strbuf_addf(out, PRI_SIZE_T_DEC, a);
    else
        strbuf_addf(out, "0x" PRI_SIZE_T_HEX, a);
};

// regs must be sorted before!
// if limit==0, then there are no limit
void make_compact_list_of_REGs (REG *regs, unsigned regs_total, strbuf *out, unsigned limit)
{
    if (limit>0 && regs_total>limit)
    {
        assert (limit>=2);
        unsigned part_length=limit/2;
        make_compact_list_of_REGs (regs, part_length, out, 0);
        strbuf_addf (out, " (%d items skipped) ", regs_total-part_length*2);
        make_compact_list_of_REGs (regs+regs_total-part_length, part_length, out, 0);
        return;
    }

    for(unsigned i=0; i < regs_total; i++)
    {
        unsigned step=0;
        if (i+2<regs_total)
            for (step=8; step!=0; step=step>>1) // 8, 4, 2, 1
                if (regs[i]+step==regs[i+1] && regs[i+1]+step==regs[i+2])
                {
                    unsigned i_start=i;
                    REG n=regs[i];
                    for (; n+step==regs[i+1] && i<regs_total; i++)
                        n=regs[i+1];

                    make_REG_compact_hex(regs[i_start], out);
                    strbuf_addstr (out, "..");
                    make_REG_compact_hex(regs[i], out);
                    if (step>1)
                        strbuf_addf (out, "(step=%d)", step);

                    break;
                };

        if (step==0)
            make_REG_compact_hex(regs[i], out);

        if (i+1 < regs_total)
            strbuf_addstr (out, ", ");
    };
};

void regcomp_or_die (regex_t *_Restrict_ preg, const char *_Restrict_ pattern, int cflags)
{
    int rc=regcomp(preg, pattern, cflags);
    if (rc!=0)
    {
        char buffer[100];
        regerror(rc, preg, buffer, 100);
        die("Regular expression compiling failed for pattern '%s' (%s)", pattern, buffer);
    };
};

