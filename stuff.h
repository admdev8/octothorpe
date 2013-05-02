#pragma once

#include <stdlib.h>
#include "datatypes.h"

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

#ifdef  __cplusplus
}
#endif
