#!/bin/bash
set -e

TMPFILE=$(mktemp)

./memutils_test > $TMPFILE
ec=$(diff -b ${srcdir}/memutils_test.correct $TMPFILE)
rm $TMPFILE

exit $ec
