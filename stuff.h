#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "datatypes.h"
#include <string.h>
#include "strbuf.h"

#ifdef  __cplusplus
extern "C" {
#endif

unsigned most_significant_hex_number(octabyte x);
void die (const char * fmt, ...);
void* memdup (void *p, size_t s);
void print_string_range (const char *s, int b, int e);
byte* load_file_or_die (const char* fname, size_t *fsize);
char* str_trim_one_char_right (char *in);
void debugger_breakpoint();
void fill_by_tetrabytes (void* ptr, size_t size, tetrabyte val);
FILE *fopen_or_die(const char* fname, const char* mode);
int stricmp_range (const char *s1, int s1_begin, int s1_end, const char *s2);
void make_REG_compact_hex (REG a, strbuf* out);
void make_compact_list_of_REGs (REG *regs, unsigned regs_total, strbuf *out, unsigned limit);

//#ifndef _MSC_VER
//#define stricmp strcasecmp
//#endif

#ifdef  __cplusplus
}
#endif
