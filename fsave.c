#include "fsave.h"

void dump_FSAVE_info (struct FSAVE_info *info)
{
	int i, j;
	for (i=0; i<8; i++)
	{
		printf ("ST%d=", i);
		for (j=0; j<10; j++)
			printf ("%02X ", info->STx[i][j]);
		printf ("\n");
	};
	printf ("control_word=0x%x\n", info->control_word);
	printf ("status_word=0x%x\n", info->status_word);
	printf ("tag_word=0x%x\n", info->tag_word);
};
