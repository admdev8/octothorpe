#include <stdbool.h>
#include <stdio.h>

// copypasted from http://www.opensource.apple.com/source/QuickTimeStreamingServer/QuickTimeStreamingServer-452/CommonUtilitiesLib/base64.c
//
static const unsigned char pr2six[256] =
{
	/* ASCII table */
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
	64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
	64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
	64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};

int Base64decode(char *bufplain, const char *bufcoded)
{
	int nbytesdecoded;
	register const unsigned char *bufin;
	register unsigned char *bufout;
	register int nprbytes;

	bufin = (const unsigned char *) bufcoded;
	while (pr2six[*(bufin++)] <= 63);
	nprbytes = (bufin - (const unsigned char *) bufcoded) - 1;
	nbytesdecoded = ((nprbytes + 3) / 4) * 3;

	bufout = (unsigned char *) bufplain;
	bufin = (const unsigned char *) bufcoded;

	while (nprbytes > 4) {
		*(bufout++) =
			(unsigned char) (pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
		*(bufout++) =
			(unsigned char) (pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
		*(bufout++) =
			(unsigned char) (pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
		bufin += 4;
		nprbytes -= 4;
	}

	/* Note: (nprbytes == 1) would be an error, so just ingore that case */
	if (nprbytes > 1) {
		*(bufout++) =
			(unsigned char) (pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
	}
	if (nprbytes > 2) {
		*(bufout++) =
			(unsigned char) (pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
	}
	if (nprbytes > 3) {
		*(bufout++) =
			(unsigned char) (pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
	}

	*(bufout++) = '\0';
	nbytesdecoded -= (4 - nprbytes) & 3;
	return nbytesdecoded;
}

/*
	likely_base64_string("0123456");
	likely_base64_string("aaaaaaa");
	likely_base64_string("aaAA11");
	likely_base64_string("aaaaAAAA1111");
	likely_base64_string("aaaaaAAAAA11");
*/
bool likely_base64_string (char* s)
{
	//printf ("%s() s=%s\n", __FUNCTION__, s);
	size_t len=0;
	size_t lower_case=0, upper_case=0, digits=0;
	
	for (; *s; s++)
	{
		len++;
		if (*s>='A' && *s<='Z')
			upper_case++;
		if (*s>='a' && *s<='z')
			lower_case++;
		if (*s>='0' && *s<='9')
			digits++;
	};
	// two times lower than distribution of characters in base64(random_data)
	float latin_expected_value=26.0/64.0/2.0;
	float digit_expected_value=10.0/64.0/2.0;

	//printf ("latin_expected_value=%f\n", latin_expected_value);
	//printf ("digits_expected_value=%f\n", digit_expected_value);
	//printf ("upper_case / len=%f\n", (float)upper_case / (float)len);
	//printf ("lower_case / len=%f\n", (float)upper_case / (float)len);
	//printf ("digits / len=%f\n", (float)digits / (float)len);

	if ((float)upper_case / (float)len < latin_expected_value)
		return false;
	if ((float)lower_case / (float)len < latin_expected_value)
		return false;
	if ((float)digits / (float)len < digit_expected_value)
		return false;
	return true;
};

// without padding symbol!
bool is_base64_char(char c)
{
	if (c>='A' && c<='Z')
		return true;
	if (c>='a' && c<='z')
		return true;
	if (c>='0' && c<='9')
		return true;
	if (c=='+')
		return true;
	if (c=='/')
		return true;
	return false;
};

