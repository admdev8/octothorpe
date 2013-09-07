OUTDIR=MINGW32_debug
BITS=$(echo $OUTDIR | sed 's/MINGW//' | sed 's/_debug//')

$OUTDIR/dmalloc_test > dmalloc_test.1
grep "main" dmalloc_test.1 > dmalloc_test.2
diff dmalloc_test.correct dmalloc_test.2
rm dmalloc_test.1
rm dmalloc_test.2

$OUTDIR/lisp_test.exe > lisp_test.1
diff lisp_test.correct lisp_test.1
rm lisp_test.1

$OUTDIR/logging_test.exe > logging_test.1
diff logging_test.correct logging_test.1
rm logging_test.1

$OUTDIR/strbuf_test.exe > strbuf_test.1
diff strbuf_test.correct strbuf_test.1
rm strbuf_test.1

$OUTDIR/string_list_test.exe > string_list_test.1
diff string_list_test.correct string_list_test.1
rm string_list_test.1

$OUTDIR/test-regex.exe > test-regex.1
diff test-regex.correct test-regex.1
rm test-regex.1

$OUTDIR/testrbtree > testrbtree.test
diff testrbtree.test.correct testrbtree.test
rm testrbtree.test

$OUTDIR/stuff_test > stuff_test.1
diff stuff_test.1 stuff_test.correct_$BITS
rm stuff_test.1

$OUTDIR/dlist_test > dlist_test.1
diff dlist_test.1 dlist_test.correct
rm dlist_test.1
