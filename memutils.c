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
