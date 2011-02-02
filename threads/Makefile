CC_OPTS = -Wall -Werror -lev -ggdb3 -I./include

all: threads

clean:
	rm -rf *.bin *.dSYM

threads:
	$(CC) $(CC_OPTS) -o hello-threads.bin hello-threads.c
	$(CC) $(CC_OPTS) -o flag-lock.bin flag-lock.c
