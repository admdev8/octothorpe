#!/bin/bash
set -e

ec=0

if [ "$OCTOTHORPE_DEBUG" ]
then 
	TMPFILE=$(mktemp)
	
	./dmalloc_test > $TMPFILE

	if [ $(grep "seq_n:2, size: 124, filename: ../../tests/dmalloc_test.c:31, func: main, struct: block124" $TMPFILE) == 1 ]
	then
		ec=1
	fi

	if [ $(grep "seq_n:3, size: 12, filename: ../../tests/dmalloc_test.c:33, func: main, struct: block12" $TMPFILE) == 1 ]
	then
		ec=1
	fi

	if [ $(grep "seq_n:4, size: 555, filename: ../../tests/dmalloc_test.c:35, func: main, struct: block555" $TMPFILE) == 1 ]
	then
		ec=1
	fi

	rm $TMPFILE
fi

exit $ec
