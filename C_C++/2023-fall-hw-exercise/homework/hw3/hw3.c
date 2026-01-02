//Bella Lu xlu62

#include <stdio.h>
#include <string.h>

#include "functions.h"


#define MAX_WORD_NUM 1024
#define MAX_INPUT_SIZE 128
#define DEFAULT_RESTRICTION 10

int main(int argc, const char *argv[]) {
  char words[ MAX_WORD_NUM ][ MAX_WORD_SIZE + 1 ]; 
  FILE* input = fopen(argv[1], "r");
  int count = 0;
  int size = 0;
  int restriction = 0;
  if (argc < 2 || argc > 3) {
    fprintf(stderr, "Error: invalid command line argument\n");
    return 4;
  } 
  if (input == NULL) {
    fprintf(stderr, "Error: could not open input file\n");
    return 2;
  }
  if (ferror(input)) {
      fprintf(stderr, "Error: error indicator was set for input file\n");
      return 3;
    }
  if (argc == 3) {
    restriction = (*argv)[2];
  } else {
    restriction = DEFAULT_RESTRICTION; 
  }
  if (fscanf(input, "%d", &size) != 1) {
    fprintf(stderr, "Error: invalid size.\n");
    return 1;
  }
  if (size < 0 || size > 1024) {
    fprintf(stderr, "Error: invalid size.\n");
    return 1;
  }
  if (read_file(input, words, size)) {
    fprintf(stderr, "Error: could not read file\n");
    return 5;
  }
  char regex[MAX_WORD_SIZE];
  scanf(" %s", regex);
  for (int i = 0; i < size; i++) {
    if (match(regex, words[i], restriction)) {
      printf("%s\n", words[i]);
      count++;
    }
  }
  if (count == 0) {
    fprintf(stdout, "No match found for regex '%s'\n", regex);
  }
  return 0;
}
