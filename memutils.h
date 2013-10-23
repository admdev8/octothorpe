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

#pragma once

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdio.h>
#include "datatypes.h"

	// to add: wydefill
	void bytefill (void* ptr, size_t size, byte val);
	void tetrabytefill (void* ptr, size_t size, tetrabyte val);

#ifndef bzero
	void bzero (void* ptr, size_t s);
#endif

	bool is_blk_zero (void *ptr, size_t s);
	bool is_element_in_array (unsigned e, unsigned* array, size_t size);

#ifdef  __cplusplus
}
#endif
