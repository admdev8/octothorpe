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

// by own GNU memmem() implementation

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
		byte *new=omemmem (ptr, haystack_size-(ptr-haystack), needle, needle_size);
		if (new==NULL)
			return rt;
		// put newly found occurrence to array
		// shrink array if needed
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
	size_t tmp=find_all_needles (haystack, haystack_size, needle, needle_size, &rt);
	DFREE(tmp);
	return rt;
};

