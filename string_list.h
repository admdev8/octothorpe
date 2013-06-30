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

typedef struct _strlist_node
{
    char* s;
    struct _strlist_node *next;
} strlist_node;

#ifdef  __cplusplus
extern "C" {
#endif

void strlist_insert_at_begin(strlist_node **l, char *s);
char* strlist_get_first(strlist_node *l);
void strlist_free (strlist_node* l, void (*free_fn)(void*));

#ifdef  __cplusplus
}
#endif

