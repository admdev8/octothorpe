#include "memutils.h"

int main()
{
	if (is_element_in_array (0xF00, (unsigned[]){ 0x123, 0x456, 0x789, 0xF00 }, 4))
		printf ("true\n");
	else
		printf ("false\n");
	
	if (is_element_in_array (0xF01, (unsigned[]){ 0x123, 0x456, 0x789, 0xF00 }, 4))
		printf ("true\n");
	else
		printf ("false\n");

	return 0;
};
