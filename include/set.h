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

// representing tree as a set (without value)

#pragma once

#include "rbtree.h"
#include "strbuf.h"

#ifdef  __cplusplus
extern "C" {
#endif

void set_of_REG_to_string (rbtree *t, strbuf *out, unsigned limit);
void set_of_doubles_to_string (rbtree *t, strbuf *out, unsigned limit);
void set_of_string_to_string (rbtree *t, strbuf *out, unsigned limit);
void set_add_string_or_free (rbtree *t, char *s);

#ifdef  __cplusplus
}
#endif

