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

#include <assert.h>

#include "lisp.h"
#include "dmalloc.h"

int main()
{
    obj *o, *i;

    o=obj_octabyte(0x0123456789ABCDEF);
    obj_dump(o);
    printf ("\n");
    obj_free(o);

    o=create_list(obj_tetrabyte(1), obj_tetrabyte(2), obj_tetrabyte(3), obj_tetrabyte(4), NULL);
    obj_dump(o);
    printf ("\n");
    printf ("LENGTH=%d\n", LENGTH(o));
    obj_free(o);

    o=create_list(
            cons(obj_tetrabyte(1), obj_cstring("hello1")), 
            cons(obj_tetrabyte(2), obj_cstring("hello2")), 
            cons(obj_tetrabyte(3), obj_cstring("hello3")),
            NULL);

    obj_dump(o);
    printf ("\n");

    // enum objects in list
    for (i=o; i; i=cdr(i))
        printf ("%d, %s\n", obj_get_as_tetrabyte(car(car(i))), obj_get_as_cstring(cdr(car(i))));

    obj_free(o);
    
    dump_unfreed_blocks();
    return 0;
};
