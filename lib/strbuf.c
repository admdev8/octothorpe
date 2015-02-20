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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "oassert.h"
#include <inttypes.h>

#include "fmt_utils.h"
#include "oassert.h"
#include "strbuf.h"
#include "dmalloc.h"
#include "stuff.h"

char* strbuf_dummybuf="\x00";

void strbuf_init (strbuf *sb, size_t size)
{
	// beware: sb->buf may contain some garbage like 0xcccccccc
#if 0
	if (sb->buf && sb->buf!=strbuf_dummybuf)
	{
		//DFREE (sb->buf); // do not do this. existing strbuf shouldn't be reinitialized!
		oassert(!"strbuf is already have something");
	};
#endif
	sb->buf=DMALLOC(char, size, "strbuf");
	if (size>0)
		sb->buf[0]=0;
	sb->strlen=0;
	sb->buflen=size;
};

void strbuf_deinit(strbuf *sb)
{
	if (sb->buf && sb->buf!=strbuf_dummybuf)
		DFREE(sb->buf);
};

void strbuf_reinit(strbuf *sb, size_t size)
{
	strbuf_deinit (sb);
	strbuf_init (sb, size);
};

void strbuf_grow (strbuf *sb, size_t size)
{
	char* new_buf;

	if (size < (sb->buflen - sb->strlen))
	{
		//printf ("(no need to reallocate)\n");
		return; // we have space already
	};

	new_buf=DMALLOC(char, sb->strlen + size + 1, "strbuf"); // FIXME: realloc or DREALLOC should be here for clarity
	if (sb->buf)
	{
		memcpy (new_buf, sb->buf, sb->strlen+1);
		if (sb->buf!=strbuf_dummybuf)
			DFREE(sb->buf);
	};
	sb->buf=new_buf;
	sb->buflen=sb->strlen + size + 1;
};

void strbuf_addstr_range (strbuf *sb, const char *s, int len)
{
	// FIXME: rework this function using strbuf_addstr_range_be()
	strbuf_grow (sb, len+1);
	memcpy (sb->buf + sb->strlen, s, len);
	sb->strlen+=len;
	sb->buf[sb->strlen]=0;
};

void strbuf_addstr (strbuf *sb, const char *s)
{
	oassert (s);
	strbuf_addstr_range (sb, s, strlen(s));
};

void strbuf_addstr_range_be (strbuf *sb, const char *s, unsigned begin, unsigned end)
{
	int i;

	oassert (begin<end);

	// FIXME: to be optimized

	for (i=begin; i<end; i++)
		strbuf_addc (sb, s[i]); 
};

void strbuf_addc (strbuf *sb, char c)
{
	strbuf_grow(sb, 1);
	sb->buf[sb->strlen]=c;
	sb->strlen++;
	sb->buf[sb->strlen]=0;
};

bool strbuf_replace_if_possible (strbuf *sb, const char *s1, const char *s2)
{
	char *t=strstr (sb->buf, s1);
	char *newbuf;
	unsigned newbuf_cursize=0, newbuf_newsize;

	if (t==NULL)
		return false;

	// now rebuild buf

	newbuf_newsize=sb->strlen-strlen(s1)+strlen(s2)+1;
	newbuf=DMALLOC(char, newbuf_newsize, "strbuf::buf"); // C++ style naming? haha

	// 1st part
	newbuf_cursize=t-sb->buf;
	memcpy (newbuf, sb->buf, newbuf_cursize);

	// 2nd part
	memcpy (newbuf+newbuf_cursize, s2, strlen(s2));
	newbuf_cursize+=strlen(s2);

	// 3rd part
	memcpy (newbuf+newbuf_cursize, t+strlen(s1), newbuf_newsize-newbuf_cursize);
	newbuf[newbuf_newsize-1]=0;

	DFREE (sb->buf);
	sb->buf=newbuf;
	sb->buflen=newbuf_newsize;
	sb->strlen=newbuf_newsize-1;

	return true;
};

void strbuf_vaddf (strbuf *sb, const char *fmt, va_list va)
{
	va_list va2;
	va_copy (va2, va);

#if defined(TARGET_IS_WINDOWS_2000) || defined (__APPLE__)
	// temporary (?) kludge
	char tmpbuf[1024];
	size_t sz=vsnprintf (tmpbuf, sizeof(tmpbuf), fmt, va2);
	oassert (sz<sizeof(tmpbuf)-1);
// FIXME: this
#elif __linux__ || __CYGWIN__
	size_t sz=vsnprintf (NULL, 0, fmt, va2);
#else
	size_t sz=_vscprintf (fmt, va2); // MSVC-specific? absent in Windows 2000 msvcrt.dll :( fuck.
#endif

	strbuf_grow(sb, sz);

#ifdef _MSC_VER
	if (vsnprintf_s (sb->buf + sb->strlen, sz+1, sz, fmt, va)==-1) // MSVC-specific
#else
		if (vsnprintf (sb->buf + sb->strlen, sz+1, fmt, va)==-1)
#endif
		{
			oassert(0);
			fatal_error();
		};
	sb->strlen+=sz;
};

void strbuf_addf (strbuf *sb, const char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	strbuf_vaddf(sb, fmt, va);
	va_end(va);
};

void make_uint32_compact (tetrabyte a, strbuf* out)
{
	if (a<10)
		strbuf_addf(out, "%d", a);
	else
		strbuf_addf(out, "0x%x", a);
};

void make_uint64_compact (octabyte a, strbuf* out)
{
	if (a<10)
		strbuf_addf (out, "%" PRId64, a);
	else
		strbuf_addf (out, "0x%" PRIx64, a);
};

void make_SIZE_T_compact (size_t a, strbuf* out)
{
	if (sizeof(size_t)==sizeof(octabyte))
		make_uint64_compact (a, out);
	else if (sizeof(size_t)==sizeof(tetrabyte))
		make_uint32_compact (a, out);
	else
	{
		oassert (0);
	};
};

void strbuf_asmhex(strbuf *out, octabyte v)
{
	if (v<10)
		strbuf_addf(out, "%d", v);
	else
		if (most_significant_hex_number(v)<=9)
			strbuf_addf (out, "%" PRIX64 "h", v);
		else
			strbuf_addf (out, "0%" PRIX64 "h", v);
};

// TODO: remove it
void strbuf_puts (strbuf *sb)
{
	puts (sb->buf);
};

void strbuf_fprint (strbuf *sb, FILE *f)
{
	fprintf (f, "%s", sb->buf);
};

void strbuf_fprint_short (strbuf *sb, unsigned limit, FILE *f)
{
	if (sb->strlen<limit)
		strbuf_fprint(sb, f);
	else
	{
		int size_of_part=limit/2-8; // let's pretend "...(%d skipped)..." string has size ~16 bytes
		oassert(size_of_part>0);
		fprintf (f, "%.*s...(%d skipped)...%.*s", 
			 size_of_part, sb->buf,
			 size_of_part,
			 size_of_part, sb->buf + sb->strlen - size_of_part);
	};
};

void strbuf_addc_C_escaped (strbuf *s, char c, bool treat_any_as_binary)
{
	if (treat_any_as_binary)
		strbuf_addf (s, "\\x%02X", (unsigned char)c);
	else
	{
		switch (c)
		{
		case '\n':
			strbuf_addstr (s, "\\n");
			break;
		case '\r':
			strbuf_addstr (s, "\\r");
			break;
		case '"':
			strbuf_addstr (s, "\\\"");
			break;
		case '\\':
			strbuf_addstr (s, "\\\\");
			break;
		default:
			if (c>0 && c<0x20)
				strbuf_addf (s, "\\x%02X", c);
			else
				strbuf_addc (s, c);
			break;
		};
	};
};

void strbuf_cvt_to_C_string (const char *in, strbuf *out, bool treat_as_binary)
{
	size_t len=strlen(in);

	for (size_t i=0; i<len; i++)
		strbuf_addc_C_escaped (out, in[i], treat_as_binary);
};

//#if defined(__linux__) || defined(__APPLE__)
extern char** environ;
//#endif

// replace %substring% to environment variable, if possible
void env_vars_expansion(strbuf *sb)
{
	int i=0;
	while (environ[i])
	{
		char *s=DSTRDUP (environ[i], "env");
		char *s1=strtok (s, "=");
		char *s2=strtok (NULL, "=");
		strbuf percented_env_var=STRBUF_INIT;
		strbuf_addc (&percented_env_var, '%');
		strbuf_addstr (&percented_env_var, s1);
		strbuf_addc (&percented_env_var, '%');

		strbuf_replace_if_possible (sb, percented_env_var.buf, s2);

		strbuf_deinit(&percented_env_var);
		DFREE(s);
		i++;
	};
};

char *strbuf_detach(strbuf *s, size_t *out_size)
{
	char *rt=s->buf;

	if (out_size)
		*out_size=s->strlen;

	s->buf=NULL;
	s->buflen=s->strlen=0;

	return rt;
};

char strbuf_last_char (strbuf *s)
{
	oassert (s->strlen!=0);
	return s->buf[s->strlen-1];
};

void strbuf_trim_last_char (strbuf *s)
{
	oassert (s->strlen!=0 && "input string is empty");
	s->strlen--;
	s->buf[s->strlen]=0;
};

void strbuf_trim_chars_at_end (strbuf *s, const char *chars)
{
	while (strchr (chars, strbuf_last_char(s)))
		strbuf_trim_last_char(s);
};

void strbuf_make_shorter (strbuf *sb, unsigned new_size)
{
	oassert (new_size < sb->strlen);
	sb->buf[new_size]=0;
	sb->strlen=new_size;
};

void strbuf_trim_string_with_comment (strbuf *sb, unsigned size, const char *str)
{
	if (sb->strlen<=size)
		return;

	strbuf_make_shorter(sb, size - strlen(str));
	strbuf_addstr (sb, str);
};

#define KB 1000
#define MB 1000000
#define GB 1000000000
//#define TB 1000000000000

void strbuf_fancy_size (strbuf* out, size_t size)
{
	/*
	  if (size>TB)
	  {
	  strbuf_addf (out, "%.1fTB", (double)size/(double)TB);
	  return;
	  };
	*/
	if (size>GB)
	{
		strbuf_addf (out, "%.1fGB", (double)size/(double)GB);
		return;
	};
	
	if (size>MB)
	{
		strbuf_addf (out, "%.1fMB", (double)size/(double)MB);
		return;
	};

	if (size>KB)
	{
		strbuf_addf (out, "%.1fKB", (double)size/(double)KB);
		return;
	};

	strbuf_addf (out, "%d", size);
};

void strbuf_add_space_if_not_empty (strbuf* out)
{
	if (out->strlen)
		strbuf_addc (out, ' ');
};
