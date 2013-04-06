dmalloc.obj: dmalloc.c dmalloc.h
	cl.exe dmalloc.c /D_DEBUG /c /Zi

memutils.obj: memutils.c memutils.h
	cl.exe memutils.c /D_DEBUG /c /Zi

rbtree.obj: rbtree.c rbtree.h
	cl.exe rbtree.c /D_DEBUG /c /Zi

rand.obj: rand.c rand.h
	cl.exe rand.c /D_DEBUG /c /Zi

strbuf.obj: strbuf.c strbuf.h
	cl.exe strbuf.c /D_DEBUG /c /Zi

stuff.obj: stuff.c stuff.h
	cl.exe stuff.c /D_DEBUG /c /Zi

logging.obj: logging.c logging.h
	cl.exe logging.c /D_DEBUG /c /Zi

octothorped.lib: dmalloc.obj memutils.obj rbtree.obj rand.obj strbuf.obj stuff.obj logging.obj
	lib.exe dmalloc.obj memutils.obj rbtree.obj rand.obj strbuf.obj stuff.obj logging.obj /OUT:octothorped.lib

testrbtree.exe: testrbtree.c octothorped.lib
	cl testrbtree.c /D_DEBUG octothorped.lib

strbuf_test.exe: strbuf_test.c octothorped.lib
	cl strbuf_test.c /D_DEBUG octothorped.lib

logging_test.exe: logging_test.c octothorped.lib
	cl logging_test.c /Zi /D_DEBUG octothorped.lib

all: octothorped.lib testrbtree.exe strbuf_test.exe logging_test.exe

clean:
	del *.lib *.exe *.obj *.asm
