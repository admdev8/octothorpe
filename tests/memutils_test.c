#include <string.h>
#include "oassert.h"
#include "memutils.h"
#include "fmt_utils.h"
#include "dmalloc.h"

void find_all_needles_test(char* haystack)
{
	size_t rt_size;
	size_t* rt=find_all_needles ((byte*)haystack, strlen(haystack), "123", 3, &rt_size);
	printf ("rt_size=" PRI_SIZE_T "\n", rt_size);
	for (size_t i=0; i<rt_size; i++)
		printf (PRI_SIZE_T ": " PRI_SIZE_T " [%s]\n", i, rt[i], &haystack[rt[i]]);
	DFREE(rt);
};

void find_all_needles_tests()
{
	find_all_needles_test("hello 123 hh 123 qj 123 lql 123");
	find_all_needles_test("123 hh 123 qj 123 lql 123 haha");
	find_all_needles_test("ooo 123 hh 123 qj 123 lql 123 haha");
};

int main()
{
	char *buf1="123456789";
	char *buf2="123 hello 123 oh oh 123";
	char *buf3="begin 123 hello 123 oh oh 123 end";

	oassert (omemmem_count ((byte*)buf1, strlen(buf1), (byte*)"--", 2)==0);
	oassert (omemmem_count ((byte*)buf2, strlen(buf2), (byte*)"123", 3)==3);
	oassert (omemmem_count ((byte*)buf3, strlen(buf3), (byte*)"123", 3)==3);

	find_all_needles_tests();
	
	dump_unfreed_blocks();

	return 0;
};
