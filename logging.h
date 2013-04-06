/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil -*- */

#pragma once

//#include <string>
//#include <iostream>
//#include <fstream>

//#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include "bool.h"

// TODO: logging flags

//extern int L_verbose_level;

// taken from z3\src\util\util.h
//#define IF_VERBOSE(LVL, CODE) { if (L_verbose_level >= LVL) { CODE } } ((void) 0)
//#define IF_TRUE(var, CODE) { if (var) { CODE } } ((void) 0)

extern BOOL L_timestamp;
extern BOOL L_quiet;

//string gen_timestamp();
void L_init (const char* fname);
//void L (int level, const char * fmt, ...);
void L_va (const char * fmt, va_list va);
void L (const char * fmt, ...);
//void L (int level, string s);
//void L (string s);
void L_once_va (const char * fmt, va_list va);
void L_once (const char * fmt, ...);
void L_print_buf_ofs (uint8_t *buf, size_t size, size_t ofs);
void L_print_buf (uint8_t *buf, size_t size);
void L_print_bufs_diff (uint8_t *buf1, uint8_t *buf2, size_t size);
