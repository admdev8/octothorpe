#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#include "datatypes.h"
#include "strbuf.h"

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct _fds
{
    FILE* fd1;
    FILE* fd2;
} fds;

// TODO: logging flags

// taken from z3\src\util\util.h
//#define IF_VERBOSE(LVL, CODE) { if (L_verbose_level >= LVL) { CODE } } ((void) 0)

extern bool L_timestamp;
extern bool L_quiet;
extern fds cur_fds;

//string gen_timestamp();
void L_init (const char* fname);
//void L (int level, const char * fmt, ...);
void L_va (const char * fmt, va_list va);
void L_fds_va (fds *s, const char * fmt, va_list va);
void L (const char * fmt, ...);
void L_fds (fds *s, const char * fmt, ...);
void L_fds_strbuf (fds *s, strbuf *sb);
//void L (int level, string s);
//void L (string s);
void L_once_va (const char * fmt, va_list va);
void L_once (const char * fmt, ...);
void L_print_buf_ofs (byte *buf, size_t size, size_t ofs);
void L_print_buf (byte *buf, size_t size);
void L_print_bufs_diff (byte *buf1, byte *buf2, size_t size);
void L_deinit (void);

#ifdef  __cplusplus
}
#endif
