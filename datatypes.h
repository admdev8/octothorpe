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

#include <stdint.h>

// D.Knuth's MMIX datatypes

typedef uint8_t byte;
typedef uint16_t wyde;
typedef uint32_t tetrabyte;
typedef int32_t tetrabyte_s;
typedef uint64_t octabyte;
typedef int64_t octabyte_s;

#ifdef __GNUC__
#define OCTABYTE_1 1LL
#endif
#ifdef _MSC_VER
#define OCTABYTE_1 1i64
#endif

#ifdef _WIN64
#define REG_1 OCTABYTE_1
typedef octabyte REG;
typedef octabyte_s SIGNED_REG;
#define REG_SIZE 8
#else
#define REG_1 1
typedef tetrabyte REG;
typedef tetrabyte_s SIGNED_REG;
#define REG_SIZE 4
#endif

#ifdef _WIN64
typedef octabyte address;
typedef octabyte_s address_offset;
#else
typedef tetrabyte address;
typedef tetrabyte_s address_offset;
#endif

/* vim: set expandtab ts=4 sw=4 : */
