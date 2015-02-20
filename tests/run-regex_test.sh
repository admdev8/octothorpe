#!/bin/bash
set -e

TMPFILE=$(mktemp)

./regex_test > $TMPFILE
ec=$(diff -b ${srcdir}/regex_test.correct $TMPFILE)
rm $TMPFILE

exit $ec
