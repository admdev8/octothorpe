dmalloc.obj: dmalloc.c dmalloc.h
	cl.exe dmalloc.c /D_DEBUG /c /Zi

memutils.obj: memutils.c memutils.h
	cl.exe memutils.c /D_DEBUG /c /Zi

rbtree.obj: rbtree.c rbtree.h
	cl.exe rbtree.c /D_DEBUG /c /Zi

rand.obj: rand.c rand.h
	cl.exe rand.c /D_DEBUG /c /Zi

octothorped.lib: dmalloc.obj memutils.obj rbtree.obj rand.obj
	lib.exe dmalloc.obj memutils.obj rbtree.obj rand.obj /OUT:octothorped.lib

all: octothorped.lib

clean:
	del *.lib *.exe *.obj *.asm
