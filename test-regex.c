#include "regex.h"
#include "dmalloc.h"

void print_string_range (char *s, int b, int e)
{
    int i;
    for (i=b; i<e; i++)
        putc(s[i], stdout);
};

int main(int argc, char **argv)
{
    char *pat = "^config=([^;]*)(;.*)?$";
    char *str = "config=asdasdasd; comment";
    
    regex_t r;
    regmatch_t m[2];

    if (regcomp(&r, pat, REG_EXTENDED | REG_NEWLINE))
        die("failed regcomp() for pattern '%s'", pat);
    if (regexec(&r, str, 2, m, 0))
        die("no match of pattern '%s' to string '%s'", pat, str);
    else
    {
        printf ("matched:\n");
        printf ("%d, %d\n", m[0].rm_so, m[0].rm_eo);
        printf ("["); print_string_range(str, m[0].rm_so, m[0].rm_eo); printf ("]\n");
        printf ("%d, %d\n", m[1].rm_so, m[1].rm_eo);
        printf ("["); print_string_range(str, m[1].rm_so, m[1].rm_eo); printf ("]\n");
    };

    //printf ("r=0x%p\n", &r);
    regfree (&r);

    dump_unfreed_blocks();
    dmalloc_deinit();

    return 0;
}
