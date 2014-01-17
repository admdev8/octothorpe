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

#include "arch.h"

#ifdef __GNUC__
#include <inttypes.h>
#endif

#ifdef O_BITS64
#define PRI_SIZE_T_HEX "%" PRIx64
#define PRI_SIZE_T_HEX_PAD "%016" PRIx64
#define PRI_SIZE_T_DEC "%" PRId64
#elif defined O_BITS32
#define PRI_SIZE_T_HEX "%x"
#define PRI_SIZE_T_HEX_PAD "%08x"
#define PRI_SIZE_T_DEC "%d"
#else
#error "O_BITS64 or O_BITS32 should be defined"
#endif

#define PRI_REG_HEX     PRI_SIZE_T_HEX
#define PRI_REG_HEX_PAD PRI_SIZE_T_HEX_PAD
#define PRI_REG_DEC     PRI_SIZE_T_DEC

#define PRI_ADR_HEX     PRI_SIZE_T_HEX
#define PRI_ADR_HEX_PAD PRI_SIZE_T_HEX_PAD
#define PRI_ADR_DEC     PRI_SIZE_T_DEC

/* vim: set expandtab ts=4 sw=4 : */
