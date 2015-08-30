#!/bin/bash
set -e

TMPFILE=$(mktemp)

./dlist_test > $TMPFILE
ec=$(diff -b ${srcdir}/dlist_test.correct $TMPFILE)
rm $TMPFILE

exit $ec
