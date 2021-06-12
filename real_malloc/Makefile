CFLAGS=-Wall -O3 -lm
SRCS=main.c malloc.c simple_malloc.c

malloc_challenge.bin : ${SRCS} Makefile
	$(CC) $(CFLAGS) -o $@ $(SRCS)

malloc_challenge_with_trace.bin : ${SRCS} Makefile
	$(CC) $(CFLAGS) -DENABLE_MALLOC_TRACE -o $@ $(SRCS)

run : 
	valgrind ./malloc_challenge.bin

run_trace : malloc_challenge_with_trace.bin
	./malloc_challenge_with_trace.bin

clean :
	-rm malloc_challenge.bin

commit :
	clang-format -i *.c
	git add .
	git commit

build :
	gcc -g -Wall -O3 -o malloc_challenge.bin main.c malloc.c simple_malloc.c -lm

