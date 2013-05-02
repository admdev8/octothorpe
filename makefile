CC=gcc
CPPFLAGS=-D_DEBUG
#CPPFLAGS=
CFLAGS=-Wall -g $(CPPFLAGS)
COMPILE_ONLY_CFLAGS=-c $(CFLAGS)
LDFLAGS=
SOURCES=dmalloc.c memutils.c rbtree.c rand.c strbuf.c stuff.c logging.c x86.c string_list.c \
	elf.c lisp.c
TEST_SOURCES=testrbtree.c strbuf_test.c logging_test.c dmalloc_test.c test-regex.c string_list_test.c
OBJECTS=$(SOURCES:.c=.o)
TEST_OBJECTS=$(TEST_SOURCES:.c=.o)
TEST_EXECS=$(TEST_SOURCES:.c=.exe)
LIBRARY=octothorped.a

all: $(LIBRARY) $(TEST_EXECS)

clean:
	rm $(OBJECTS)
	rm $(TEST_OBJECTS)
	rm $(LIBRARY)
	rm $(TEST_EXECS)
	rm regex.o

$(LIBRARY): $(OBJECTS) regex.o
	ar -mc $(LIBRARY) $(OBJECTS) regex.o

regex.o: regex.c dmalloc.h
	$(CC) $(COMPILE_ONLY_CFLAGS) regex.c -DUSE_DMALLOC -o $@

-include $(OBJECTS:.o=.d)

%.o: %.c
	$(CC) $(COMPILE_ONLY_CFLAGS) $*.c -o $*.o
	$(CC) -MM $(COMPILE_ONLY_CFLAGS) $*.c > $*.d

# tests:

-include $(TEST_SOURCES:.c=.d)

%.exe: %.o $(LIBRARY)
	$(CC) $(CFLAGS) $*.c $(LIBRARY) -o $@
	$(CC) -MM $(COMPILE_ONLY_CFLAGS) $*.c > $*.d

