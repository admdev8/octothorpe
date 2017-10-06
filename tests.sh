#!/bin/bash
set -e

./test1

./logging_test > /dev/null
diff -b logging_test.correct logging_test.log
rm logging_test.log

TMPFILE=$(mktemp)

./memutils_test > $TMPFILE
diff -b memutils_test.correct $TMPFILE
rm $TMPFILE

./regex_test > $TMPFILE
diff -b regex_test.correct $TMPFILE
rm $TMPFILE

./ostrings_test > $TMPFILE
diff -b ostrings_test.correct $TMPFILE
rm $TMPFILE

./strbuf_test > $TMPFILE
diff -b strbuf_test.correct $TMPFILE
rm $TMPFILE

./stuff_test > $TMPFILE
diff -b stuff_test.correct $TMPFILE
rm $TMPFILE

./string_list_test > $TMPFILE
diff -b string_list_test.correct $TMPFILE
rm $TMPFILE

./rbtree_test > $TMPFILE
diff -b rbtree_test.correct $TMPFILE
rm $TMPFILE

echo hello > tmp
ec=$(./enum_files_test | grep tmp | grep "size=6" | wc -l)
if [ $ec -ne 1 ]
then
	echo enum_files_test failed
fi
rm tmp

