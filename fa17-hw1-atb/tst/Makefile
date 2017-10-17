UNAME_S := $(shell uname -s)
CC = gcc
CFLAGS = -g -std=gnu99 -Wall -c
LDFLAGS = -g -Wall

ifeq ($(UNAME_S), Darwin)
    MEMCHECK=valgrind --tool=memcheck --leak-check=full --track-origins=yes --dsymutil=yes
endif

ifeq ($(UNAME_S), Linux)
    MEMCHECK=valgrind --tool=memcheck --leak-check=full --track-origins=yes
endif

test: clean test_tst
	./test_tst

memcheck: test
	$(MEMCHECK) ./test_tst

test_tst: test_tst.o tst.o
	$(CC) $(LDLAGS) -o test_tst test_tst.o tst.o

test_tst.o: test_tst.c
	$(CC) $(CFLAGS) test_tst.c

tst.o: tst.c tst.h
	$(CC) $(CFLAGS) tst.c

clean:
	rm -rf *.o test_tst test_tst.dSYM
