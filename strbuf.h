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
		char *buf; // in future, short string may be placed instead of pointer here, like in MSVC std::string (?)
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
	void strbuf_fprint (strbuf *sb, FILE *f);
	void strbuf_fprint_short (strbuf *sb, unsigned limit, FILE *f);
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
	void strbuf_trim_last_char (strbuf *s);
	void strbuf_trim_chars_at_end (strbuf *s, const char *chars);
	void strbuf_make_shorter (strbuf *sb, unsigned new_size);
	// if sb->strlen>size, trim it and add str at the end
	void strbuf_trim_string_with_comment (strbuf *sb, unsigned size, const char *str);
	void strbuf_fancy_size (strbuf* out, size_t size);

	// TODO: bool strbuf_eq

#ifdef  __cplusplus
}
#endif
