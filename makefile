dmalloc.obj: dmalloc.c dmalloc.h
	cl.exe dmalloc.c /D_DEBUG /c /Zi

memutils.obj: memutils.c memutils.h
	cl.exe memutils.c /D_DEBUG /c /Zi

ptrie.obj: ptrie.c ptrie.h
	cl.exe ptrie.c /D_DEBUG /c /Zi

octothorpe.lib: dmalloc.obj memutils.obj ptrie.obj
	lib.exe dmalloc.obj memutils.obj ptrie.obj /OUT:octothorpe.lib

all: octothorpe.lib

clean:
	del *.exe *.obj *.asm
