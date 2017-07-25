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

#pragma once

#include <stdbool.h>

#include "datatypes.h"

#ifdef  __cplusplus
extern "C" {
#endif

	octa uint64_log2 (octa i);
	octa ipow(octa base, octa exp);
	bool IsInteger (double d);
	bool uint64_is_2n(octa v);
	bool uint32_is_2n(tetra v);
	int is_prime(unsigned int n);

	// w=u*v, m=number of 16-bit words in u[], n=... in v[]
	void mulmns(uint16_t w[], uint16_t u[], uint16_t v[], int m, int n);

	// this function solves the equation:
	// a * x = 1 + b * 2^32
	// ... and returns a, b
	bool modinv32 (uint32_t x, uint32_t *a, uint32_t *b);
	// a * x = 1 + b * 2^64
	bool modinv64 (uint64_t x, uint64_t *a, uint64_t *b);

#ifdef  __cplusplus
}
#endif

