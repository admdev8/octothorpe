#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

#include "datatypes.h"
#include "stuff.h"

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
