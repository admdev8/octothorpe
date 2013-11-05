#include <stdio.h>
#include <time.h>

#include "enum_files.h"

void cb (const char *name, const char *pathname, time_t t, bool is_dir, void *param)
{
	struct tm *_tm=localtime(&t);
	printf ("%s() name=%s pathname=%s is_dir=%d time=%s", __FUNCTION__, name, pathname, is_dir, asctime(_tm));
};

int main()
{
	char tmp[256];
	enum_files_in_dir (getcwd (tmp, sizeof(tmp)), cb, NULL);
};
