OPTIONS=-D_DEBUG=1 -DRE_USE_MALLOC=1
OBJECTS=base64.o dlist.o dmalloc.o elf.o entropy.o entropy_int.o enum_files.o files.o fsave.o lisp.o logging.o memutils.o \
	oassert.o octomath.o ostrings.o rand.o rbtree.o regex.o set.o strbuf.o string_list.o stuff.o x86.o \
	x86_intrin.o regex_helpers.o

all: octothorpe.a tests

octothorpe.a: $(OBJECTS)
	ar r octothorpe.a $(OBJECTS)

base64.o: base64.c base64.h
	gcc $(OPTIONS) -c base64.c

dlist.o: dlist.c dlist.h
	gcc $(OPTIONS) -c dlist.c

dmalloc.o: dmalloc.c dmalloc.h
	gcc $(OPTIONS) -c dmalloc.c

elf.o: elf.c elf.h elf_structures.h
	gcc $(OPTIONS) -c elf.c

entropy.o: entropy.c entropy.h
	gcc $(OPTIONS) -c entropy.c

entropy_int.o: entropy_int.c
	gcc $(OPTIONS) -c entropy_int.c

enum_files.o: enum_files.c enum_files.h
	gcc $(OPTIONS) -c enum_files.c

files.o: files.c files.h
	gcc $(OPTIONS) -c files.c

fsave.o: fsave.c fsave.h
	gcc $(OPTIONS) -c fsave.c

lisp.o: lisp.c lisp.h
	gcc $(OPTIONS) -c lisp.c

logging.o: logging.c logging.h
	gcc $(OPTIONS) -c logging.c

memutils.o: memutils.c memutils.h
	gcc $(OPTIONS) -c memutils.c

oassert.o: oassert.c oassert.h
	gcc $(OPTIONS) -c oassert.c

octomath.o: octomath.c octomath.h
	gcc $(OPTIONS) -c octomath.c

ostrings.o: ostrings.c ostrings.h
	gcc $(OPTIONS) -c ostrings.c

rand.o: rand.c rand.h
	gcc $(OPTIONS) -c rand.c

rbtree.o: rbtree.c rbtree.h
	gcc $(OPTIONS) -c rbtree.c

regex.o: regcomp.c regex.h regex.c regex_helpers.c regex_helpers.h regex_internal.c regex_internal.h regexec.c
	gcc $(OPTIONS) -c regex.c

set.o: set.c set.h
	gcc $(OPTIONS) -c set.c

strbuf.o: strbuf.c strbuf.h
	gcc $(OPTIONS) -c strbuf.c

string_list.o: string_list.c string_list.h
	gcc $(OPTIONS) -c string_list.c

stuff.o: stuff.c stuff.h
	gcc $(OPTIONS) -c stuff.c

x86.o: x86.c x86.h
	gcc $(OPTIONS) -c x86.c

x86_intrin.o: x86_intrin.c x86_intrin.h
	gcc $(OPTIONS) -c x86_intrin.c

logging_test: logging_test.c
	gcc $(OPTIONS) logging_test.c -o logging_test octothorpe.a

memutils_test: memutils_test.c
	gcc $(OPTIONS) memutils_test.c -o memutils_test octothorpe.a

regex_test: regex_test.c
	gcc $(OPTIONS) regex_test.c -o regex_test octothorpe.a

ostrings_test: ostrings_test.c
	gcc $(OPTIONS) ostrings_test.c -o ostrings_test octothorpe.a

strbuf_test: strbuf_test.c
	gcc $(OPTIONS) strbuf_test.c -o strbuf_test octothorpe.a

stuff_test: stuff_test.c
	gcc $(OPTIONS) stuff_test.c -o stuff_test octothorpe.a

string_list_test: string_list_test.c
	gcc $(OPTIONS) string_list_test.c -o string_list_test octothorpe.a

enum_files_test: enum_files_test.c
	gcc $(OPTIONS) enum_files_test.c -o enum_files_test octothorpe.a

rbtree_test: rbtree_test.c
	gcc $(OPTIONS) rbtree_test.c -o rbtree_test octothorpe.a

tests: test1.c octothorpe.a logging_test memutils_test regex_test ostrings_test strbuf_test string_list_test rbtree_test \
	stuff_test enum_files_test
	gcc $(OPTIONS) test1.c -o test1 octothorpe.a -lm

clean:
	rm -f *.o
	rm -f octothorpe.a
	rm -f tests

