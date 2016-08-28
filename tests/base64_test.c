#include <stdio.h>
#include "base64.h"
#include "oassert.h"

int main()
{
	oassert (likely_base64_string("0123456")==0);
	oassert (likely_base64_string("aaaaaaa")==0);
	oassert (likely_base64_string("aaAA11")==1);
	oassert (likely_base64_string("aaaaAAAA1111")==1);
	oassert (likely_base64_string("aaaaaAAAAA11")==1);
};

