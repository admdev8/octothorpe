TESTS = run-strbuf_test.sh run-dlist_test.sh run-lisp_test.sh run-memutils_test.sh \
	run-ostrings_test.sh run-string_list_test.sh run-regex_test.sh run-testrbtree.sh \
	run-logging_test.sh run-stuff_test.sh run-dmalloc_test.sh

TESTS_ENVIRONMENT = BITS=$(BITS) OCTOTHORPE_DEBUG=$(OCTOTHORPE_DEBUG)

check_PROGRAMS = strbuf_test dlist_test lisp_test memutils_test ostrings_test string_list_test \
	regex_test testrbtree logging_test stuff_test dmalloc_test

strbuf_test_SOURCE = strbuf_test.c
strbuf_test_LDADD = $(top_builddir)/lib/liboctothrope.a

dlist_test_SOURCE = dlist_test.c
dlist_test_LDADD = $(top_builddir)/lib/liboctothrope.a

lisp_test_SOURCE = lisp_test.c
lisp_test_LDADD = $(top_builddir)/lib/liboctothrope.a

memutils_test_SOURCE = memutils_test.c
memutils_test_LDADD = $(top_builddir)/lib/liboctothrope.a

ostrings_test_SOURCE = ostrings_test.c
ostrings_test_LDADD = $(top_builddir)/lib/liboctothrope.a

string_list_test_SOURCE = string_list_test.c
string_list_test_LDADD = $(top_builddir)/lib/liboctothrope.a

regex_test_SOURCE = regex_test.c
regex_test_LDADD = $(top_builddir)/lib/liboctothrope.a

testrbtree_SOURCE = testrbtree.c
testrbtree_LDADD = $(top_builddir)/lib/liboctothrope.a

logging_test_SOURCE = logging_test.c
logging_test_LDADD = $(top_builddir)/lib/liboctothrope.a

stuff_test_SOURCE = stuff_test.c
stuff_test_LDADD = $(top_builddir)/lib/liboctothrope.a

dmalloc_test_SOURCE = dmalloc_test.c
dmalloc_test_LDADD = $(top_builddir)/lib/liboctothrope.a

AM_CPPFLAGS = -I$(top_srcdir)/include

EXTRA_DIST = run-dlist_test.sh run-dmalloc_test.sh run-lisp_test.sh run-logging_test.sh \
	run-memutils_test.sh run-ostrings_test.sh run-regex_test.sh run-strbuf_test.sh \
	run-string_list_test.sh run-stuff_test.sh run-testrbtree.sh \
	dlist_test.correct lisp_test.correct logging_test.correct_32 \
	logging_test.correct_64 memutils_test.correct ostrings_test.correct regex_test.correct \
	strbuf_test.correct string_list_test.correct stuff_test.correct_32 stuff_test.correct_64 \
	testrbtree.correct
