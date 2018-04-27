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
 * Written by Dennis Yurichev <dennis(a)yurichev.com>, 2013-2017
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/3.0/.
 *
 */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#include "datatypes.h"
#include "stuff.h"

// TODO support intrinsic, if possible
/*
   uint64_t uint64_log2 (uint64_t i)
   {
   oassert (i!=0);

   return __lzcnt64(i);
   };
 */
// http://stackoverflow.com/questions/11376288/fast-computing-of-log2-for-64-bit-integers
static const int tab64[64]=
{
	63,  0, 58,  1, 59, 47, 53,  2,
	60, 39, 48, 27, 54, 33, 42,  3,
	61, 51, 37, 40, 49, 18, 28, 20,
	55, 30, 34, 11, 43, 14, 22,  4,
	62, 57, 46, 52, 38, 26, 32, 41,
	50, 36, 17, 19, 29, 10, 13, 21,
	56, 45, 25, 31, 35, 16,  9, 12,
	44, 24, 15,  8, 23,  7,  6,  5
};

octa octa_log2 (octa value)
{
	value |= value >> 1;
	value |= value >> 2;
	value |= value >> 4;
	value |= value >> 8;
	value |= value >> 16;
	value |= value >> 32;
	return tab64[((octa)((value - (value >> 1))*0x07EDD5E59A4E28C2)) >> 58];
}


// http://stackoverflow.com/questions/101439/the-most-efficient-way-to-implement-an-integer-based-power-function-powint-int
// TODO check overflow
octa ipow(octa base, octa exp)
{
	octa result = 1;
	while (exp)
	{
		if (exp & 1)
			result *= base;
		exp >>= 1;
		base *= base;
	}

	return result;
}

bool IsInteger (double d)
{
	return floor(d)==d;
};

// is number in 2^n form?
// ... i.e., it has only one bit set.
bool uint64_is_2n(octa v)
{
	return popcnt64(v)==1;
};

bool uint32_is_2n(tetra v)
{
	return popcnt32(v)==1;
};

// https://rosettacode.org/wiki/Primality_by_trial_division#C
int is_prime(unsigned int n)
{
	unsigned int p;
	if (!(n & 1) || n < 2 ) return n == 2;

	/* comparing p*p <= n can overflow */
	for (p = 3; p <= n/p; p += 2)
		if (!(n % p)) return 0;
	return 1;
}

// copypasted from Hacker's Delight, Chapter 8, p.155
// w=u*v, m=number of 16-bit words in u[], n=... in v[]
void mulmns(uint16_t w[], uint16_t u[], uint16_t v[], int m, int n)
{
	unsigned int k, t, b;
	int i, j;
	for (i = 0; i < m; i++)
		w[i] = 0;
	for (j = 0; j < n; j++)
	{
		k = 0;
		for (i = 0; i < m; i++)
		{
			t = u[i]*v[j] + w[i + j] + k;
			w[i + j] = t; // (I.e., t & 0xFFFF).
			k = t >> 16;
		}
		w[j + m] = k;
	}
	// Now w[] has the unsigned product.
	// sign of w[] is not corrected
};

#define WYDES_PER_TETRA sizeof(uint32_t)/sizeof(uint16_t)
#define WYDES_PER_OCTA sizeof(uint64_t)/sizeof(uint16_t)

// reworked from https://github.com/gcc-mirror/gcc/blob/1cb6c2eb3b8361d850be8e8270c597270a1a7967/gcc/loop-iv.c#L1291
// this function solves the equation:
// a * x = 1 + b * 2^32
// ... and returns a, b
// see also: https://en.wikipedia.org/wiki/B%C3%A9zout's_identity
bool modinv32 (uint32_t x, uint32_t *a, uint32_t *b)
{
	uint32_t input_x=x, rslt=1;

	for (int i=0; i<31; i++)
	{
		rslt = (rslt * x);
		x = (x * x);
	}

	// get the second coefficient of Bézout's identity:
	uint32_t tmp[2];
	mulmns((uint16_t*)tmp, (uint16_t*)&rslt, (uint16_t*)&input_x, WYDES_PER_TETRA, WYDES_PER_TETRA);

	if (tmp[0]!=1)
		return false; // not coprimes

	*a=rslt;
	*b=tmp[1];

	return true;
}

// a * x = 1 + b * 2^64
bool modinv64 (uint64_t x, uint64_t *a, uint64_t *b)
{
	uint64_t input_x=x, rslt=1;

	for (int i=0; i<63; i++)
	{
		rslt = (rslt * x);
		x = (x * x);
	}

	// get the second coefficient of Bézout's identity:
	uint64_t tmp[2];
	mulmns((uint16_t*)tmp, (uint16_t*)&rslt, (uint16_t*)&input_x, WYDES_PER_OCTA, WYDES_PER_OCTA);

	if (tmp[0]!=1)
		return false; // not coprimes

	*a=rslt;
	*b=tmp[1];

	return true;
}

