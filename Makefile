CFLAGS = /MD /nologo /W3 /EHsc /Za /DWIN /D_CRT_SECURE_NO_DEPRECATE

build: tema1so.obj parser.obj hashmap.obj
	link /out:so-cpp.exe tema1so.obj parser.obj hashmap.obj
tema1so.obj: tema1so.c
	cl $(CFLAGS) /c tema1so.c
parser.obj: parser.c
	cl $(CFLAGS) /c parser.c
hashmap.obj: hashmap.c
	cl $(CFLAGS) /c hashmap.c
clean:
	del *.obj so-cpp.exe
