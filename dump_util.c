#include "stuff.h"
#include "logging.h"
#include "dmalloc.h"
#include "files.h"

// TODO set offset, size, etc

int main(int argc, char* argv[])
{
	printf ("Dump util. <dennis(a)yurichev.com> %s\n", __DATE__);

	if (argc!=4)
		die ("Usage: %s <filename.bin> <offset (hex)> <size (hex)>\n", argv[0]);

	FILE* f=fopen_or_die(argv[1], "rb");	

	int ofs, size;

	if (sscanf(argv[2], "%x", &ofs)!=1)
		die ("Can't parse [%s]\n", argv[2]);

	if (sscanf(argv[3], "%x", &size)!=1)
		die ("Can't parse [%s]\n", argv[3]);

	if (fseek(f, ofs, SEEK_CUR)!=0)
		die ("Seek failed\n");

	byte* buf=DMALLOC(byte, size, "buf");

	if (fread (buf, size, 1, f)!=1)
		die ("Cannot read file\n");

	L_init_stdout_only ();
	L_print_buf_ofs_C (buf, size, ofs);
	L_deinit();

	fclose (f);

	DFREE(buf);
};
