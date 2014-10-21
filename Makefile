build: tema0.exe

tema0.exe: tema0.obj hash.obj hashtable.obj
	cl /Fetema0.exe tema0.obj hash.obj hashtable.obj
	
tema0.obj: tema0.c

hash.obj: hash.c

hashtable.obj: hashtable.c

clean:
	del tema0.exe *.obj