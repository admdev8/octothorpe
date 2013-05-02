CC=gcc
CPPFLAGS=-D_DEBUG
CFLAGS=-Wall -g $(CPPFLAGS)
COMPILE_ONLY_CFLAGS=-c -Wall -D_DEBUG -g
LDFLAGS=
SOURCES=dmalloc.c memutils.c rbtree.c rand.c strbuf.c stuff.c logging.c x86.c string_list.c elf.c
TEST_SOURCES=testrbtree.c strbuf_test.c logging_test.c dmalloc_test.c test-regex.c string_list_test.c
OBJECTS=$(SOURCES:.c=.o)
TEST_OBJECTS=$(TEST_SOURCES:.c=.o)
TEST_EXECS=$(TEST_SOURCES:.c=.exe)
LIBRARY=octothorped.a

all: $(LIBRARY) $(TEST_EXECS)
	
clean:
	rm *.o
	rm *.a
	rm *.exe

$(LIBRARY): $(OBJECTS) regex.o
	ar -mc $(LIBRARY) $(OBJECTS) regex.o

regex.o: regex.c
	$(CC) $(COMPILE_ONLY_CFLAGS) regex.c -DUSE_DMALLOC -o $@

-include $(OBJECTS:.o=.d)

%.o: %.c
	$(CC) $(COMPILE_ONLY_CFLAGS) $*.c -o $*.o
	$(CC) -MM $(COMPILE_ONLY_CFLAGS) $*.c > $*.d

testrbtree.exe:
	$(CC) $(CFLAGS) testrbtree.c $(LIBRARY) -o $@
strbuf_test.exe:
	$(CC) $(CFLAGS) strbuf_test.c $(LIBRARY) -o $@
logging_test.exe:
	$(CC) $(CFLAGS) logging_test.c $(LIBRARY) -o $@
dmalloc_test.exe: dmalloc_test.c
	$(CC) $(CFLAGS) dmalloc_test.c $(LIBRARY) -o $@
test-regex.exe:
	$(CC) $(CFLAGS) test-regex.c $(LIBRARY) -o $@
string_list_test.exe:
	$(CC) $(CFLAGS) string_list_test.c $(LIBRARY) -o $@
