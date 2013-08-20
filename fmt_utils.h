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

#ifdef _WIN64
#define PRI_SIZE_T_HEX "%I64x"
#define PRI_SIZE_T_HEX_PAD "%016I64x"
#define PRI_SIZE_T_DEC "%I64d"
#else
#define PRI_SIZE_T_HEX "%x"
#define PRI_SIZE_T_HEX_PAD "%08x"
#define PRI_SIZE_T_DEC "%d"
#endif

#define PRI_REG_HEX     PRI_SIZE_T_HEX
#define PRI_REG_HEX_PAD PRI_SIZE_T_HEX_PAD
#define PRI_REG_DEC     PRI_SIZE_T_DEC

#define PRI_ADR_HEX     PRI_SIZE_T_HEX
#define PRI_ADR_HEX_PAD PRI_SIZE_T_HEX_PAD
#define PRI_ADR_DEC     PRI_SIZE_T_DEC

/* vim: set expandtab ts=4 sw=4 : */
