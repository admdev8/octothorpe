#!/bin/bash
set -e

TMPFILE=$(mktemp)

./testrbtree > $TMPFILE
ec=$(diff -b ${srcdir}/testrbtree.correct $TMPFILE)
rm $TMPFILE

exit $ec
