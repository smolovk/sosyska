libname = test

all:
	gcc -shared -o lib${libname}.so -fPIC lib.c
	ln -f lib${libname}.so ../libs/lib${libname}.so
