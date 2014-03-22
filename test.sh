OUTDIR=$1

#echo outdir=$OUTDIR

if [[ $OUTDIR == *MINGW* ]]
then
	BITS=$(echo $OUTDIR | sed 's/MINGW//' | sed 's/_debug//' | sed 's/_release//')
else
	if [[ $OUTDIR == *i686* ]]
	then
		BITS="32"
	fi
	if [[ $OUTDIR == *x86_64* ]] # not tested
	then
		BITS="64"
	fi
fi

if [[ $OUTDIR == *Darwin* ]]
then
	BITS="64"
fi

if [ -z "$BITS" ]; then echo OS is not determined; exit 0; fi

BUILD_SUFFIX=$2

$OUTDIR/dmalloc_test.exe > dmalloc_test.1
grep "main" dmalloc_test.1 > dmalloc_test.2
diff -b dmalloc_test.correct_$BUILD_SUFFIX dmalloc_test.2
rm dmalloc_test.1
rm dmalloc_test.2

$OUTDIR/lisp_test.exe > lisp_test.1
diff -b lisp_test.correct lisp_test.1
rm lisp_test.1

$OUTDIR/logging_test.exe > logging_test.1
diff -b logging_test.correct logging_test.1
rm logging_test.1

$OUTDIR/strbuf_test.exe > strbuf_test.1
diff -b strbuf_test.correct strbuf_test.1
rm strbuf_test.1

$OUTDIR/string_list_test.exe > string_list_test.1
diff -b string_list_test.correct string_list_test.1
rm string_list_test.1

$OUTDIR/test-regex.exe > test-regex.1
diff -b test-regex.correct test-regex.1
rm test-regex.1

$OUTDIR/testrbtree.exe > testrbtree.test
diff -b testrbtree.test.correct testrbtree.test
rm testrbtree.test

$OUTDIR/stuff_test.exe > stuff_test.1
diff -b stuff_test.1 stuff_test.correct_$BITS
rm stuff_test.1

$OUTDIR/dlist_test.exe > dlist_test.1
diff -b dlist_test.1 dlist_test.correct
rm dlist_test.1

$OUTDIR/memutils_test.exe > memutils_test.1
diff -b memutils_test.1 memutils_test.correct
rm memutils_test.1

$OUTDIR/ostrings_test.exe > ostrings_test.1
diff -b ostrings_test.1 ostrings_test.correct
rm ostrings_test.1
