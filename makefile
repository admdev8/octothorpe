C_OBJS=dmalloc.obj memutils.obj rbtree.obj rand.obj strbuf.obj stuff.obj logging.obj x86.obj
GNU_REGEX_C_OBJS=regex.obj regex_internal.obj
ASM_OBJS=FPU_stuff_asm.obj
TEST_EXECS=testrbtree.exe strbuf_test.exe logging_test.exe dmalloc_test.exe test-regex.exe

$(C_OBJS): $(@B).c $(@B).h
    cl.exe $(@B).c /D_DEBUG /c /Zi

$(GNU_REGEX_C_OBJS): $(@B).c
    cl.exe $(@B).c /D_DEBUG /c /Zi

$(ASM_OBJS): $(@B).asm $(@B).h
    ml.exe $(@B).asm /c

octothorped.lib: $(C_OBJS) $(ASM_OBJS) $(GNU_REGEX_C_OBJS)
	lib.exe $(C_OBJS) $(ASM_OBJS) $(GNU_REGEX_C_OBJS) /OUT:octothorped.lib

$(TEST_EXECS): $(@B).c
    cl $(@B).c /Zi /D_DEBUG octothorped.lib

all: octothorped.lib $(TEST_EXECS)

clean:
	del *.lib *.exe *.obj
