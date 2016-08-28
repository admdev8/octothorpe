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

#include "datatypes.h"
#include <stdio.h>
#include <stdbool.h>

bool file_exist (const char *filename);
bool is_file(const char* path);
bool is_dir(const char* path); 
size_t get_file_size_or_die (const char* fname);
byte* load_file_or_die (const char* fname, size_t *fsize);

// ... or return NULL
// FIXME: add OPTIONAL keyword
unsigned char* load_file (const char* fname, size_t *fsize);
void save_file_or_die (const char* fname, byte *buf, size_t fsize);

typedef void (*read_text_file_by_line_callback_fn)(char *line, void *param);
void read_text_file_by_line_or_die (char *fname, read_text_file_by_line_callback_fn cb, void *param);

// "filename.ext" -> "filename", "ext"
// no path!
void split_fname (char *fname, char *basefname, size_t basefname_len, char *ext, size_t ext_len);
int open_or_die (char *fname, int mode);

