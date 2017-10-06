// integer (fixed-point) entropy calculation
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "datatypes.h"

// Q16.16
#define PRECISION 16
#define SCALE (1U << PRECISION)

// copypasted from https://github.com/dmoulding/log2fix/blob/master/log2fix.c
// idea taken from http://www.claysturner.com/dsp/BinaryLogarithm.pdf
int32_t log2fix (uint32_t x)
{
    int32_t b = 1U << (PRECISION - 1);
    int32_t y = 0;

    if (PRECISION < 1 || PRECISION > 31) {
        return INT32_MAX; // indicates an error
    }

    if (x == 0) {
        return INT32_MIN; // represents negative infinity
    }

    while (x < 1U << PRECISION) {
        x <<= 1;
        y -= 1U << PRECISION;
    }

    while (x >= 2U << PRECISION) {
        x >>= 1;
        y += 1U << PRECISION;
    }

    uint64_t z = x;

    for (size_t i = 0; i < PRECISION; i++) {
        z = z * z >> PRECISION;
        if (z >= 2U << PRECISION) {
            z >>= 1;
            y += b;
        }
        b >>= 1;
    }

    return y;
}

// copypasted from https://spin.atomicobject.com/2012/03/15/simple-fixed-point-math/
int32_t q_mul(int32_t x, int32_t y)
{
	return ((int64_t)x * (int64_t)y) / (1 << PRECISION );
};

// copypasted from https://spin.atomicobject.com/2012/03/15/simple-fixed-point-math/
int32_t q_div(int32_t x, int32_t y)
{
	return (int64_t)x*(1<<PRECISION) / y;
};

int32_t q_make_from_integer (int i)
{
	return i<<PRECISION;
};

// this source code is based on example from http://rosettacode.org/wiki/Entropy#C

static int makehist(unsigned char *S,int *hist,int len)
{
	int wherechar[256];
	int i,histlen;
	histlen=0;
	for(i=0;i<256;i++)
		wherechar[i]=-1;
	for(i=0;i<len;i++)
	{
		if(wherechar[(int)S[i]]==-1)
		{
			wherechar[(int)S[i]]=histlen;
			histlen++;
		}
		hist[wherechar[(int)S[i]]]++;
	}
	return histlen;
}

static int32_t entropy_helper(int *hist,int histlen,int buflen)
{
	int i;
	int32_t H=0;
	for(i=0;i<histlen;i++)
	{
		int32_t tmp=q_div(q_make_from_integer(hist[i]), q_make_from_integer(buflen));
		H-=q_mul (tmp, log2fix(tmp));
	}
	return H;
}
	
// or convert to double: entropy_int(...) / SCALE
int32_t entropy_int (byte* buf, size_t bufsize)
{
	int *hist,histlen;
	
	hist=(int*)calloc(bufsize,sizeof(int));

	histlen=makehist(buf,hist,bufsize);
	//hist now has no order (known to the program) but that doesn't matter
	return entropy_helper(hist,histlen,bufsize);
}

