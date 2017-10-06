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
#include <limits.h>

// D.Knuth's MMIX datatypes

typedef uint8_t byte;
typedef uint16_t wyde;
typedef uint32_t tetra;
typedef int32_t tetra_s;
typedef uint64_t octa;
typedef int64_t octa_s;

#define WYDE_LEN_IN_BYTES sizeof(wyde)

#ifdef __GNUC__
#define OCTA_1 1LL
#elif _MSC_VER
#define OCTA_1 1i64
#else
#error "compiler was not detected"
#endif

#if __WORDSIZE==64
#define REG_1 OCTA_1
typedef octa REG;
#define REG_MSB 0x8000000000000000
typedef octa_s SIGNED_REG;
#define REG_MAX UINT64_MAX
#define REG_SIZE 8
typedef octa address;
typedef octa_s address_offset;
#elif __WORDSIZE==32
#define REG_1 1
typedef tetra REG;
#define REG_MSB 0x80000000
typedef tetra_s SIGNED_REG;
#define REG_MAX UINT32_MAX
#define REG_SIZE 4
#else
#error "__WORDSIZE is undefined"
typedef tetra address;
typedef tetra_s address_offset;
#endif

