# -DUSE_DMALLOC is only for regex
CPPFLAGS=-D_DEBUG -DUSE_DMALLOC
CFLAGS=-c -Wall -g
SOURCES=dmalloc.c memutils.c rbtree.c rand.c strbuf.c stuff.c logging.c x86.c string_list.c \
	elf.c lisp.c regex.c
TEST_SOURCES=testrbtree.c strbuf_test.c logging_test.c dmalloc_test.c test-regex.c \
	string_list_test.c
OBJECTS=$(SOURCES:.c=.o)
DEPFILES=$(SOURCES:.c=.d)
TEST_EXECS=$(TEST_SOURCES:.c=.exe)
LIBRARY=octothorped.a

all: $(LIBRARY)($(OBJECTS)) $(TEST_EXECS) $(DEPFILES)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(TEST_OBJECTS)
	$(RM) $(LIBRARY)
	$(RM) $(TEST_EXECS)

-include $(OBJECTS:.o=.d)

%.d: %.c
	$(CC) -MM $(CFLAGS) $(CPPFLAGS) $< > $@

# tests:

-include $(TEST_SOURCES:.c=.d)

%.exe: %.o $(LIBRARY)
	$(CC) $< $(LIBRARY) -o $@
