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

#include "dmalloc.h"
#include "datatypes.h"
#include "logging.h"

int main()
{
    byte *t1, *t2, *t3, *t4, *t5, *t6, *t7;
    char *s;
    //L_init("tmp");
    
    s=DSTRDUP ("hahaha", "s");

    t1=DMALLOC(byte, 6, "block123");
    t2=DMALLOC(byte, 124, "block124");
    memcpy (t2, "block124", 8);
    t3=DMALLOC(byte, 12,  "block12");
    memcpy (t3, "block12", 7);
    t4=DMALLOC(byte, 555, "block555");
    memcpy (t4, "block555", 8);
    t5=DMALLOC(byte, 666, "block666");
    t6=DMALLOC(byte, 777, "block777");
    t7=DMALLOC(byte, 0, "empty block");

    t6=DREALLOC(t6, byte, 888, "new size for block777");

    // guards are checking?
    //memcpy (t1, "12345678", 8);
    //memcpy((byte*)t1-1, "a", 1);

    t1=DREALLOC(t1, byte, 100, "new size for block123");

    DFREE(t1);
    DFREE(t5);
    //L_print_buf (t5, 666);
    DFREE(t6);
    DFREE(t7);
    DFREE(s);

    // make compiler happy about unused variables!
    t1=t4; t1=t3; t1=t2;

    dump_unfreed_blocks();
    dmalloc_deinit();

    return 0;
};
