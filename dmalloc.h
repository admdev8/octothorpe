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

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

void* dmalloc (size_t size, const char * filename, unsigned line, const char * function, const char * structname);
#define DMALLOC(type, size, comment) ((type*)dmalloc(sizeof(type)*size, __FILE__, __LINE__, __func__, comment))

void* drealloc (void* ptr, size_t size, const char * filename, unsigned line, const char * function, const char * structname);
#define DREALLOC(ptr, type, size, comment) ((type*)drealloc(ptr, sizeof(type)*size, __FILE__, __LINE__, __func__, comment))

void* dcalloc (size_t size, const char * filename, unsigned line, const char * function, const char * structname);
#define DCALLOC(type, size, comment) ((type*)dcalloc(sizeof(type)*size, __FILE__, __LINE__, __func__, comment))

void dfree (void* ptr);

// for symmetry!
#define DFREE(p) (dfree(p))

char* dstrdup (const char *str, const char * filename, unsigned line, const char * function, const char * structname);
#define DSTRDUP(str,comment) (dstrdup(str, __FILE__, __LINE__, __func__, comment))

void* dmemdup (void *p, size_t s, const char * filename, unsigned line, const char * function, const char * structname);

#define DMEMDUP(ptr,size,comment) (dmemdup(ptr, size, __FILE__, __LINE__, __func__, comment))

void dump_unfreed_blocks();
void dmalloc_deinit();

void dmalloc_break_at_seq_n (unsigned seq_n);

void* memdup_range (void *s, int begin, size_t size);
char *strdup_range (const char *s, int begin, size_t size);

#ifdef  __cplusplus
}
#endif

/* vim: set expandtab ts=4 sw=4 : */
