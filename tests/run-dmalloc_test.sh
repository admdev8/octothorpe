#!/bin/bash
set -e

if [ -z "$OCTOTHORPE_DEBUG" ]
then 
	# nothing to test
	ec=0
else
	TMPFILE=$(mktemp)
	
	./dmalloc_test | grep "main" > $TMPFILE
	ec=$(diff -b ${srcdir}/dmalloc_test.correct $TMPFILE)
	rm $TMPFILE
fi

exit $ec
