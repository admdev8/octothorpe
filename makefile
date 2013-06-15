CC=gcc
# -DUSE_DMALLOC is only for regex
CPPFLAGS=-D_DEBUG -DUSE_DMALLOC
#CFLAGS=-c -Wall -g -std=c99
CFLAGS=-c -Wall -g -std=gnu99
SOURCES=dmalloc.c memutils.c rbtree.c rand.c strbuf.c stuff.c logging.c x86.c string_list.c \
	elf.c lisp.c regex.c dlist.c FPU_stuff_GCC.c files.c set.c
TEST_SOURCES=testrbtree.c strbuf_test.c logging_test.c dmalloc_test.c test-regex.c \
	string_list_test.c lisp_test.c stuff_test.c dlist_test.c
DEPFILES=$(SOURCES:.c=.d)
OUTDIR=$(MSYSTEM)_debug
OBJECTS=$(addprefix $(OUTDIR)/,$(SOURCES:.c=.o))
TEST_OBJECTS=$(addprefix $(OUTDIR)/,$(TEST_SOURCES:.c=.o))
TEST_EXECS=$(addprefix $(OUTDIR)/,$(TEST_SOURCES:.c=.exe))
LIBRARY=$(OUTDIR)\octothorped.a

all: $(OUTDIR) $(LIBRARY)($(OBJECTS)) $(TEST_EXECS) $(DEPFILES) $(OUTDIR)/lisp.o

$(OUTDIR):
	mkdir $(OUTDIR)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(TEST_OBJECTS)
	$(RM) $(LIBRARY)
	$(RM) $(TEST_EXECS)
	$(RM) *.d

-include $(OBJECTS:.o=.d)

$(OUTDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

%.d: %.c
	$(CC) -MM $(CFLAGS) $(CPPFLAGS) $< > $@

# tests:

%.exe: %.o $(LIBRARY)
	$(CC) $< $(LIBRARY) -o $@
