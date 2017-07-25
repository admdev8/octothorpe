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

#include "datatypes.h"
#include "dmalloc.h"
#include "stuff.h"
#include "logging.h"
#include "memutils.h"
#include "oassert.h"

int main()
{
	byte *buf=DMALLOC (byte, 1024, "buf");
	L_init ("tmp");

	bytefill (buf, 1024, 0);
	tetrafill(buf, 128, 0x0badf00d);
	L_print_buf (buf, 256);
	bytefill (buf, 1024, 0);
	tetrafill(buf, 127, 0x0badf00d);
	L_print_buf (buf, 256);
	bytefill (buf, 1024, 0);
	tetrafill(buf, 126, 0x0badf00d);
	L_print_buf (buf, 256);
	bytefill (buf, 1024, 0);
	tetrafill(buf, 125, 0x0badf00d);
	L_print_buf (buf, 256);

	strbuf sb=STRBUF_INIT;
	REG a[12]={2, 4, 5, 6, 7, 8, 0xa, 0xb, 0xf, 0x13, 0x29a, 0x29b};
	make_compact_list_of_REGs (a, 12, &sb, 0);
	printf ("%s\n", sb.buf);
	strbuf_reinit(&sb, 0);

	REG b[17]={2, 4, 8, 0xC, 0x10, 0x11, 0x12, 0x13, 0x14, 0x18, 0x1c, 0x29b, 0x300, 0x308, 0x310, 0x318, 0x320};

	make_compact_list_of_REGs (b, 17, &sb, 0);
	printf ("%s\n", sb.buf);
	strbuf_reinit(&sb, 0);

	make_compact_list_of_REGs (b, 17, &sb, 7);
	printf ("%s\n", sb.buf);
	strbuf_deinit(&sb);

	oassert (CRC32((byte*)"123456789", 9, 0)==0xcbf43926);
	oassert (CRC32((byte*)"123456789", 9, 0x11223344)==0xd360c638);

	// popcnt32()
	oassert(popcnt32(0)==0);
	oassert(popcnt32(1)==1);
	oassert(popcnt32(0x8000)==1);
	oassert(popcnt32(0xFFFFFFFF)==32);
	// popcnt64()
	oassert(popcnt64(0)==0);
	oassert(popcnt64(1)==1);
	oassert(popcnt64(0x8000)==1);
	oassert(popcnt64(0xFFFFFFFF)==32);
	oassert(popcnt64(0x8000000000000000UL)==1);
	oassert(popcnt64(0xFFFFFFFFFFFFFFFFUL)==64);
};
