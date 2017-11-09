#include "stuff.h"
#include "logging.h"
#include "dmalloc.h"
#include "files.h"
#include "fmt_utils.h"
#include "memutils.h"

// TODO C-strings support

int main(int argc, char* argv[])
{
	printf ("Replace util. <dennis(a)yurichev.com> %s\n", __DATE__);

	if (argc!=4)
	{
		printf ("Usage: %s <filename.bin> string_to_replace string_to_put_there\n", argv[0]);
		printf ("Terminating zero isn't used!\n");
		return 0;
	};

	size_t fsize;
	byte* buf=load_file_or_die(argv[1], &fsize);

	char* s1=argv[2];
	char* s2=argv[3];

	if (strlen(s1)!=strlen(s2))
		die ("Strings has different size: [%s] and [%s]\n", s1, s2);

	size_t needles_t;
	size_t* needles=find_all_needles (buf, fsize, s1, strlen(s1), &needles_t);

	for (int i=0; i<needles_t; i++)
	{
		printf ("Found [%s] at 0x" PRI_REG_HEX ", put [%s] here.\n", s1, needles[i], s2);
		memcpy(buf+needles[i], s2, strlen(s2));
	};

	save_file_or_die(argv[1], buf, fsize);

	DFREE(buf);
};
