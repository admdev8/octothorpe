#!/bin/bash
set -e

TMPFILE=$(mktemp)

./ostrings_test > $TMPFILE
ec=$(diff -b ${srcdir}/ostrings_test.correct $TMPFILE)
rm $TMPFILE

exit $ec
