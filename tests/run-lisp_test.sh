#!/bin/bash
set -e

TMPFILE=$(mktemp)

./lisp_test > $TMPFILE
ec=$(diff -b ${srcdir}/lisp_test.correct $TMPFILE)
rm $TMPFILE

exit $ec
