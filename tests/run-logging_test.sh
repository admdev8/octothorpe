#!/bin/bash
set -e

TMPFILE=$(mktemp)

./logging_test > $TMPFILE
ec=$(diff -b ${srcdir}/logging_test.correct_$BITS $TMPFILE)
rm $TMPFILE
rm logging_test.log

exit $ec
