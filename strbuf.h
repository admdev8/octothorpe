// My own C string library

// inspired by git strbuf

// RATIONALE: string buffer should be used in place where one need to modify strings or
// construct them in place

#pragma once

typedef struct _strbuf
{
    char *buf; // in future, short string may be placed instead of pointer here (?)
    unsigned strlen; // known string length (without trailing zero)
    unsigned buflen; // allocated buffer length
} strbuf;

void strbuf_init (strbuf *sb, size_t size);
void strbuf_deinit(strbuf *sb);
void strbuf_grow (strbuf *sb, size_t size);
void strbuf_addstr_range (strbuf *sb, char *s, int len);
void strbuf_addstr (strbuf *sb, char *s);
void strbuf_addc (strbuf *sb, char c);
void strbuf_vaddf (strbuf *sb, const char *fmt, va_list va);
void strbuf_addf (strbuf *sb, const char *fmt, ...);

