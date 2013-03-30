// My own C string library

// inspired by git strbuf

// RATIONALE: string buffer should be used in places where one need to modify strings or
// construct them 

#pragma once

#include <stdio.h>
#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct _strbuf
{
    char *buf; // in future, short string may be placed instead of pointer here (?)
    unsigned strlen; // known string length (without trailing zero)
    unsigned buflen; // allocated buffer length
} strbuf;

void strbuf_init (strbuf *sb, size_t size);
void strbuf_deinit(strbuf *sb);
void strbuf_grow (strbuf *sb, size_t size);
void strbuf_addstr_range (strbuf *sb, const char *s, int len);
void strbuf_addstr (strbuf *sb, const char *s);
void strbuf_addc (strbuf *sb, char c);
void strbuf_vaddf (strbuf *sb, const char *fmt, va_list va);
void strbuf_addf (strbuf *sb, const char *fmt, ...);

void make_uint32_compact (uint32_t a, strbuf* out);
void make_uint64_compact (uint64_t a, strbuf* out);
void make_SIZE_T_compact (size_t a, strbuf* out);

void strbuf_asmhex(strbuf *out, uint64_t v);

// TODO: BOOL strbuf_eq

#ifdef  __cplusplus
}
#endif
