#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

unsigned most_significant_hex_number(uint64_t x)
{
    uint64_t t=x;
    int i;

    // _BitScanReverse64 can be used here, probably (?)

    for (i=0; i<(64/4); i++)
    {
        if (t&0xF000000000000000)
        {
            //printf (__FUNCTION__"(0x%016llX) -> 0x%X\n", x, t>>(64-4));
            return t>>(64-4);
        }
        else
            t=t<<4;
    };
    //printf (__FUNCTION__"(0x%016llX) -> 0\n", x);
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

