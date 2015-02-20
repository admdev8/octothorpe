#!/bin/bash
set -e

TMPFILE=$(mktemp)

./string_list_test > $TMPFILE
ec=$(diff -b ${srcdir}/string_list_test.correct $TMPFILE)
rm $TMPFILE

exit $ec
