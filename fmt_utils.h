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

#include <limits.h>

#ifdef __GNUC__
#include <inttypes.h>
#endif

#define PRI_OCTA_HEX "%" PRIx64
#define PRI_OCTA_HEX_PAD "%016" PRIx64
#define PRI_OCTA_DEC "%" PRId64
#define PRI_TETRA_HEX "%x"
#define PRI_TETRA_HEX_PAD "%08x"
#define PRI_TETRA_DEC "%d"

// limits.h in GNU has __WORDSIZE
#if __WORDSIZE==64
#define PRI_SIZE_T_HEX      PRI_OCTA_HEX
#define PRI_SIZE_T_HEX_PAD  PRI_OCTA_HEX_PAD
#define PRI_SIZE_T_DEC      PRI_OCTA_DEC
#elif __WORDSIZE==32
#define PRI_SIZE_T_HEX      PRI_TETRA_HEX
#define PRI_SIZE_T_HEX_PAD  PRI_TETRA_HEX_PAD
#define PRI_SIZE_T_DEC      PRI_TETRA_DEC
#else
#error "__WORDSIZE is undefined"
#endif

#define PRI_REG_HEX     PRI_SIZE_T_HEX
#define PRI_REG_HEX_PAD PRI_SIZE_T_HEX_PAD
#define PRI_REG_DEC     PRI_SIZE_T_DEC

#define PRI_ADR_HEX     PRI_SIZE_T_HEX
#define PRI_ADR_HEX_PAD PRI_SIZE_T_HEX_PAD
#define PRI_ADR_DEC     PRI_SIZE_T_DEC

// both MSVC and MinGW
#ifdef _WIN32
#define PRI_SIZE_T "%Iu"
#define PRI_SSIZE_T "%Id"
#elif defined(__GNUC__)
#define PRI_SIZE_T "%zu"
#define PRI_SSIZE_T "%zd"
#else
#error "unknown compiler"
#endif

/* vim: set expandtab ts=4 sw=4 : */
