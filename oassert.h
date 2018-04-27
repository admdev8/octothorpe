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

#include "stuff.h"

// Rationale: my own oassert with <s>with blackjack and hookers</s> _Noreturn C11 keyword and __FUNCTION__.

#ifdef _DEBUG
#define oassert(_Expression) \
 (void) \
 ((!!(_Expression)) || \
  (_oassert(#_Expression,__FILE__,__LINE__,__FUNCTION__),0))

#else
#define oassert(_Expression) ((void)0)
#endif

#define fatal_error() _fatal_error(__FILE__, __LINE__, __func__)

#ifdef  __cplusplus
extern "C" {
#endif

MY_NORETURN void _oassert (const char *msg, const char *file, unsigned line, const char *func);
MY_NORETURN void _fatal_error (const char *file, unsigned line, const char *func);

#ifdef  __cplusplus
}
#endif

