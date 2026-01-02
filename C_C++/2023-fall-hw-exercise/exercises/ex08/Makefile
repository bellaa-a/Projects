# Sample Makefile

CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra

# Links together files needed to create executable
run_concat: run_concat.o string_functions.o
	$(CC) -o main main_file.o functions.o

# Compiles mainFile.c to create mainFile.o
# Note that we list functions.h here as a file that
# main_file.c depends on, since main_file.c #includes it
run_concat.o: run_concat.c string_functions.h
	$(CC) $(CFLAGS) -c run_concat.c

# Compiles functions.c to create functions.o
# Note that we list functions.h here as a file that
# functions.c depends on, since functions.c #includes it
string_functions.o: string_functions.c string_functions.h
	$(CC) $(CFLAGS) -c string_functions.c

# Removes all object files and the executable named main,
# so we can start fresh
clean:
	rm -f *.o run_concat
