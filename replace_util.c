#include <stdbool.h>

#include "stuff.h"
#include "logging.h"
#include "dmalloc.h"
#include "files.h"
#include "fmt_utils.h"
#include "memutils.h"

// TODO C-strings support

bool prefix(const char *prefix, const char *str)
{
	return strncmp(prefix, str, strlen(prefix)) == 0;
}

char* to_wchar(char* in)
{
	size_t len=(strlen(in)+1)*2;
	char* rt=malloc(len);

	for (int i=0; in[i]; i++)
		rt[i*2]=in[i];

	return rt;		
};

int main(int argc, char* argv[])
{
	printf ("Replace util. <dennis(a)yurichev.com> %s\n", __DATE__);

	printf ("Usage: %s --find=string_to_find --replace=string_to_put_there [OPTIONS] <filename.bin>\n", argv[0]);
	printf ("Options:\n");
	printf ("--put-zero-byte\tPut it at EOL. Otherwise, string isn't terminated, to your own risk.\n");
	printf ("\n");

	char* s_find=NULL;
	char* s_replace=NULL;
	bool put_zero_byte=false;
	bool unicode=false;

	for (int i=1; i<argc; i++)
	{
		//printf ("%s\n", argv[i]);
		if (memcmp(argv[i], "--", 2)==0)
		{
			if (prefix("--find=", argv[i]))
				s_find=argv[i]+strlen("--find=");
			if (prefix("--replace=", argv[i]))
				s_replace=argv[i]+strlen("--replace=");
			if (strcmp("--put-zero-byte", argv[i])==0)
				put_zero_byte=true;
			if (strcmp("--unicode", argv[i])==0)
				unicode=true;
		}
		else
		{
			// file name
			char* fname=argv[i];
			if (s_find==NULL)
				die ("--find option hasn't been set");
			if (s_replace==NULL)
				die ("--replace option hasn't been set");

			printf ("s_find: %s\n", s_find);
			printf ("s_replace: %s\n", s_replace);

			size_t s_find_size=strlen(s_find);
			size_t s_replace_size=strlen(s_replace);
			size_t zero_bytes=1;

			if (unicode)
			{
				s_find=to_wchar(s_find);
				s_find_size=s_find_size*2;
				s_replace=to_wchar(s_replace);
				s_replace_size=s_replace_size*2;
				zero_bytes=zero_bytes*2;
			};

			size_t fsize;
			byte* buf=load_file_or_die(fname, &fsize);

			size_t needles_t;
			size_t* needles=find_all_needles (buf, fsize, s_find, s_find_size, &needles_t);

			printf ("needles_t=%d\n", needles_t);

			for (int i=0; i<needles_t; i++)
			{
				printf ("Found [%s] at 0x" PRI_REG_HEX ", we put [%s] here.\n", s_find, needles[i], s_replace);
				memcpy(buf+needles[i], s_replace, s_replace_size + (put_zero_byte ? zero_bytes : 0));
			};

			save_file_or_die(fname, buf, fsize);

			DFREE(buf);
		}
	};
};
