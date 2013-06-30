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

#include "logging.h"
#include "dmalloc.h"

int main()
{
    L_init("logging_test.log");
    L_once("#1 should be printed once\n");
    L_once("#2 should be printed once\n");
    L_once("#3 should be printed once\n");
    L_once("#1 should be printed once\n");
    L_once("#1 should be printed once\n");
    L_once("#1 should be printed once\n");
    L_once("#2 should be printed once\n");
    L_once("#2 should be printed once\n");
    L_once("#3 should be printed once\n");
    L_deinit();

    dump_unfreed_blocks();
    return 0;
};
