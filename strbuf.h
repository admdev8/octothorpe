// My own C string library

// inspired by git strbuf

// RATIONALE: string buffer should be used in places where one need to modify strings or
// construct them 

#pragma once

#include <stdio.h>
#include "datatypes.h"
#include <stdarg.h>
#include <stdbool.h>

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct _strbuf
{
    char *buf; // in future, short string may be placed instead of pointer here (?)
    unsigned strlen; // known string length (without trailing zero)
    unsigned buflen; // allocated buffer length
} strbuf;

extern char* strbuf_dummybuf;

#define STRBUF_INIT { strbuf_dummybuf, 0, 0 }

void strbuf_init (strbuf *sb, size_t size);
void strbuf_deinit(strbuf *sb);
void strbuf_reinit(strbuf *sb, size_t size);
void strbuf_grow (strbuf *sb, size_t size);
void strbuf_addstr_range (strbuf *sb, const char *s, int len);
// idea: could be renamed to strbuf_adds()
void strbuf_addstr (strbuf *sb, const char *s);
void strbuf_addstr_range_be (strbuf *sb, const char *s, unsigned begin, unsigned end);
void strbuf_addc (strbuf *sb, char c);
void strbuf_vaddf (strbuf *sb, const char *fmt, va_list va);
void strbuf_addf (strbuf *sb, const char *fmt, ...);
void strbuf_puts (strbuf *sb);
bool strbuf_replace_if_possible (strbuf *sb, const char *s1, const char *s2);

void make_uint32_compact (tetrabyte a, strbuf* out);
void make_uint64_compact (octabyte a, strbuf* out);
void make_SIZE_T_compact (size_t a, strbuf* out);

void strbuf_asmhex(strbuf *out, octabyte v);

void strbuf_addc_C_escaped (strbuf *s, char c, bool treat_any_as_binary);
void strbuf_cvt_to_C_string (const char *in, strbuf *out, bool treat_as_binary);

void env_vars_expansion(strbuf *sb);

char *strbuf_detach(strbuf *s, size_t *out_size);
char strbuf_last_char (strbuf *s);

// TODO: bool strbuf_eq

#ifdef  __cplusplus
}
#endif
