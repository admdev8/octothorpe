/*
 *             _        _   _                           
 *            | |      | | | |                          
 *   ___   ___| |_ ___ | |_| |__   ___  _ __ _ __   ___ 
 *  / _ \ / __| __/ _ \| __| '_ \ / _ \| '__| '_ \ / _ \
 * | (_) | (__| || (_) | |_| | | | (_) | |  | |_) |  __/
 *  \___/ \___|\__\___/ \__|_| |_|\___/|_|  | .__/ \___|
 *                                          | |         
 *                                          |_|
 *
 * Written by Dennis Yurichev <dennis(a)yurichev.com>, 2013
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/3.0/.
 *
 */

#include "regex.h"
#include "dmalloc.h"
#include "stuff.h"

void tst2()
{
    const char* CFG_PAT="^trace_skip=([^!; ]*)!([^!; ]*)!([^!; ]*)([[:space:]])?(;.*)?$";
    regex_t trace_skip_pat;
    const char* buf=//"trace_skip=.*!netbios.dll!.* ; comment here\n";
    "trace_skip=\\%SystemRoot%\\System32.*!.*dll!.* ; skip all functions in 64-bit system DLLs\n";
    regmatch_t matches[4];
    int i, rc;
    char buffer[100];

    if ((rc=regcomp(&trace_skip_pat, CFG_PAT, REG_EXTENDED | REG_ICASE | REG_NEWLINE))!=0)
    {
        regerror(rc, &trace_skip_pat, buffer, 100);
        die("failed regcomp() for pattern '%s' (%s)", CFG_PAT, buffer);
    };

    printf ("dummy regexec() call -> %d\n", regexec (&trace_skip_pat, "\n", 4, matches, 0));
    printf ("dummy regexec() call -> %d\n", regexec (&trace_skip_pat, "", 4, matches, 0));

    if (regexec (&trace_skip_pat, buf, 4, matches, 0)==0)
    {
        for (i=0; i<4; i++)
        {
            printf ("%d %d [", matches[i].rm_so, matches[i].rm_eo); 
            print_string_range(buf, matches[i].rm_so, matches[i].rm_eo-matches[i].rm_so);
            printf ("]\n");
        };
        //printf ("\n");
    };

    regfree (&trace_skip_pat);
};

int main(int argc, char **argv)
{
    char *pat = "^config=([^;]*)(;.*)?$";
    char *str = "config=asdasdasd; comment";
    int  rc;
    char buffer[100];
    
    regex_t r;
    regmatch_t m[2];

    if ((rc=regcomp(&r, pat, REG_EXTENDED | REG_NEWLINE))!=0)
    {
        regerror(rc, &r, buffer, 100);
        die("failed regcomp() for pattern '%s' (%s)", pat, buffer);
    }
    if (regexec(&r, str, 2, m, 0))
        die("no match of pattern '%s' to string '%s'", pat, str);
    else
    {
        printf ("matched:\n");
        printf ("%d, %d\n", m[0].rm_so, m[0].rm_eo);
        printf ("["); print_string_range(str, m[0].rm_so, m[0].rm_eo-m[0].rm_so); printf ("]\n");
        printf ("%d, %d\n", m[1].rm_so, m[1].rm_eo);
        printf ("["); print_string_range(str, m[1].rm_so, m[1].rm_eo-m[1].rm_so); printf ("]\n");
    };

    //printf ("r=0x%p\n", &r);
    regfree (&r);

    tst2();

    dump_unfreed_blocks();
    dmalloc_deinit();

    return 0;
}

/* vim: set expandtab ts=4 sw=4 : */
