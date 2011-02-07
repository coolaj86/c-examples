CC_OPTS = -Wall -Werror -lev -ggdb3 -I./include -lpthread

ifeq ($(shell uname), Darwin)
LIBFLAG=
else
LIBFLAG=-Wl
endif


all: pseudo-fd

clean:
	rm -f pseudo-fd-test.bin

pseudo-fd:
	$(CC) $(CC_OPTS) -o pseudo-fd-test.bin pseudo-fd-test.c pseudo-fd.c
