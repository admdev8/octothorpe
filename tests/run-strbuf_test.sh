#!/bin/bash
set -e

TMPFILE=$(mktemp)

./strbuf_test > $TMPFILE
ec=$(diff -b ${srcdir}/strbuf_test.correct $TMPFILE)
rm $TMPFILE

exit $ec
