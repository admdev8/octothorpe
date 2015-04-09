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

#include <stdbool.h>
#include <memory.h>
#include "datatypes.h"
#include "stuff.h"
#include "oassert.h"
#include "dmalloc.h"

void bytefill (void* ptr, size_t size, byte val)
{
	memset(ptr, val, size);
};

void tetrabytefill (void* ptr, size_t size, tetrabyte val)
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

#ifndef bzero
void bzero (void* ptr, size_t s)
{
	bytefill(ptr, s, 0);
};
#endif

// TODO: should be optimized
bool is_blk_zero (void *ptr, size_t s)
{
	byte *p=(byte*)ptr;
	for (int i=0; i<s; i++)
		if (p[i])
			return false;
	return true;
};

// my own temporary GNU memmem() implementation
// TODO rework

byte *omemmem (byte *haystack, size_t haystack_size, byte *needle, size_t needle_size)
{
	if (needle_size > haystack_size)
		return NULL;

	// may be optimized, probably...
	for (size_t i=0; i<haystack_size - needle_size + 1; i++)
	{
		if (memcmp (haystack+i, needle, needle_size)==0)
			return haystack+i;
	};
	return NULL;
};

// Knuth–Morris–Pratt algorithm
// copypasted from http://cprogramming.com/snippets/source-code/knuthmorrispratt-kmp-string-search-algorithm
const byte *kmp_search(const byte *haystack, size_t haystack_size, const byte *needle, size_t needle_size)
{
	int *T;
	int i, j;
	const byte *result = NULL;
 
	if (needle_size==0)
		return haystack;
 
	/* Construct the lookup table */
	T = (int*) malloc((needle_size+1) * sizeof(int)); // FIXME use DMALLOC
	T[0] = -1;
	for (i=0; i<needle_size; i++)
	{
		T[i+1] = T[i] + 1;
		while (T[i+1] > 0 && needle[i] != needle[T[i+1]-1])
			T[i+1] = T[T[i+1]-1] + 1;
	}
 
	/* Perform the search */
	for (i=j=0; i<haystack_size; )
	{
		if (j < 0 || haystack[i] == needle[j])
		{
			++i, ++j;
			if (j == needle_size) 
			{
				result = haystack+i-j;
				break;
			}
		}
		else j = T[j];
	}
 
	free(T); // FIXME use DFREE
	return result;
}
// like omemmem, but find all occurrences
size_t* find_all_needles (byte *haystack, size_t haystack_size, byte* needle, size_t needle_size, 
		OUT size_t* rt_size)
{
	oassert(rt_size);
	size_t* rt=DMALLOC(size_t, 1, "size_t (1)");
	size_t rt_allocated=1;
	*rt_size=0;

	for (byte* ptr=haystack; ptr < (haystack+haystack_size); *rt_size=(*rt_size)+1)
	{
		//byte *new=omemmem (ptr, haystack_size-(ptr-haystack), needle, needle_size);
		byte *new=kmp_search (ptr, haystack_size-(ptr-haystack), needle, needle_size);
		if (new==NULL)
			return rt;
		// put newly found occurrence to array
		// expand array if needed
		if (*rt_size == rt_allocated)
		{
			rt=DREALLOC (rt, size_t, rt_allocated*2, "size_t");
			rt_allocated*=2;
		};
		size_t pos=new-haystack;
		rt[*rt_size]=pos;
		ptr=new+needle_size;
	};
	return rt;
};

size_t omemmem_count (byte *haystack, size_t haystack_size, byte *needle, size_t needle_size)
{
	size_t rt;
	size_t *tmp=find_all_needles (haystack, haystack_size, needle, needle_size, &rt);
	DFREE(tmp);
	return rt;
};

void XOR_block (byte* a, byte* b, size_t s)
{
	// TODO SIMD?
	for (int i=0; i<s; i++)
		a[i]^=b[i];
};

