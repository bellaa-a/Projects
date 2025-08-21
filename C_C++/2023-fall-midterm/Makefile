# Homework 3
GCC = gcc
CONSERVATIVE_FLAGS = -std=c99 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

puzzle: puzzle.o puzzle_funcs.o ppm_io.o
	$(GCC) -o puzzle puzzle.o puzzle_funcs.o ppm_io.o

puzzle_tests: puzzle_tests.o puzzle_funcs.o ppm_io.o
	$(GCC) -g -o puzzle_tests puzzle_tests.o puzzle_funcs.o ppm_io.o
	./puzzle_tests

puzzle_tests.o: puzzle_tests.c puzzle.h ppm_io.h
	$(GCC) -c puzzle_tests.c $(CFLAGS)

puzzle.o: puzzle.c puzzle.h ppm_io.h
	$(GCC) -c puzzle.c $(CFLAGS)

puzzle_funcs.o: puzzle_funcs.c puzzle.h ppm_io.h
	$(GCC) -c puzzle_funcs.c

ppm_io.o: ppm_io.c ppm_io.h
	$(GCC) -c ppm_io.c
  
clean:
	rm -f *.o puzzle puzzle_test*~ 