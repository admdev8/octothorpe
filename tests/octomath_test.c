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

#include "octomath.h"
#include "oassert.h"

int main()
{
	// uint64_log2() tests:
	oassert (uint64_log2(1)==0);
	oassert (uint64_log2(2)==1);
	oassert (uint64_log2(4)==2);
	oassert (uint64_log2(8)==3);
	oassert (uint64_log2(65536)==16);
	oassert (uint64_log2(0xFFFFFFFFFFFFFFFF)==63);

	oassert(is_prime(131071)==1); // https://oeis.org/A000043
	oassert(is_prime(131071+1)==0);
};
