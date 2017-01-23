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

typedef struct _dlist
{
    struct _dlist* prev;
    struct _dlist* next;
    void* data;
} dlist;

#ifdef  __cplusplus
extern "C" {
#endif

dlist *dlist_init();
void dlist_insert_at_begin(dlist **l, void *data);
void* dlist_get_first(dlist *l);
void dlist_free(dlist *l, void (*free_fn)(void*));
void dlist_unlink(dlist **lst, dlist *l);
void dlist_dump(dlist *l);
void dlist_check_consistency(dlist *l);

#ifdef  __cplusplus
}
#endif

