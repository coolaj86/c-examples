CFLAGS=-Wall -Werror -fPIC
#LDFLAGS=-shared
CC=gcc ${CFLAGS} ${LDFLAGS}

all: a b dynaload

a:
	${CC} -o liba.o -c liba.c
	${CC} -shared -soname,liba.so.1 -o liba.so.1.0 liba.o
	ln -sf liba.so.1.0 liba.so

b:
	${CC} -o libb.o -c libb.c
	${CC} -shared -soname,libb.so.1 -o libb.so.1.0 libb.o
	ln -sf libb.so.1.0 libb.so

dynaload:
	${CC} -o dynaload.bin dynaload.c

clean:
	rm -rf dynaload.bin *.so.* *.so *.o

.PHONY: all a b
