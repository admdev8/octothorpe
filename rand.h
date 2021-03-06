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

#include <stdbool.h>
#include <stdlib.h>

#include "datatypes.h"

#ifdef  __cplusplus
extern "C" {
#endif

void sgenrand (unsigned long seed);
unsigned long genrand();
uint64_t genrand64();
// [begin, end]
int rand_reg (int begin, int end);
double rand_double ();
bool rand_bernoulli_distribution (float probability_of_1);
void rand_buf(byte* out, size_t size);

#ifdef  __cplusplus
}
#endif

/* vim: set expandtab ts=4 sw=4 : */
