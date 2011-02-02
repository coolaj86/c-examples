CC_OPTS = -Wall -Werror -lev -ggdb3 -I./include -lpthread

all: threads

clean:
	rm -f threads-test fake-file

threads:
	$(CC) $(CC_OPTS) -o threads-test threads-test.c
	$(CC) $(CC_OPTS) -o psuedo-fd-test psuedo-fd-test.c
