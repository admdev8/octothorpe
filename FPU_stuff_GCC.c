#include "FPU_stuff.h"

double __cdecl cvt80to64 (byte *p)
{
    return (double)*((long double*)p);
};

void __cdecl cvt64to80 (double in, byte *out)
{
    *((long double*)out)=in;
};

