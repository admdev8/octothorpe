#!/bin/bash
set -e

TMPFILE=$(mktemp)

./stuff_test > $TMPFILE
ec=$(diff -b ${srcdir}/stuff_test.correct_$BITS $TMPFILE)
rm $TMPFILE
rm tmp

exit $ec
