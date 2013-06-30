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

#include "string_list.h"
#include "dmalloc.h"

int main()
{
    strlist_node *l=NULL;
    strlist_node *i;

    strlist_insert_at_begin (&l, DSTRDUP("1st", "string"));
    strlist_insert_at_begin (&l, DSTRDUP("2nd", "string"));
    strlist_insert_at_begin (&l, DSTRDUP("3rd", "string"));

    printf ("strlist_get_first=%s\n", strlist_get_first(l));

    printf ("enumerate:\n");

    for (i=l; i->next!=NULL; i=i->next)
        printf ("%s\n", i->s);

    strlist_free(l, dfree);
    
    dump_unfreed_blocks();

    return 0;
};
