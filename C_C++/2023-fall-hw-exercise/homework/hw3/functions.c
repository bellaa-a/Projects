//Bella Lu xlu62

#include "functions.h"

#include <stdio.h>
#include <string.h>

int read_file(FILE *fp, char words[][MAX_WORD_SIZE + 1], int size) {

  for (int i = 0; i < size; i++) {
    int returnValue = fscanf(fp, " %s", words[i]);
    if (returnValue == EOF) {
      fprintf(stderr, "Error: could not parse line\n");
      return 1;
    }
  }
  return 0;
}

int match(const char *regex, const char *word, int restriction) {

  if (strlen(regex) == 0 && strlen(word) > 0) {
    return 0;
  }
  if (strlen(regex) == 0 && strlen(word) == 0) {
    return 1;
  }
  if (regex[1] == '*') {
    if (word[0] == regex[0]) {
      if (match(regex, word + 1, restriction))
        return 1;
    }
    if (match(regex + 2, word, restriction)) {
      return 1;
    }
  } else if (regex[1] == '?') {
    if (word[0] == regex[0]) {
      if (match(regex + 2, word + 1, restriction)) {
      return 1;
      }
    } 
    if (match(regex + 2, word, restriction)) {
      return 1;
    }
  } else if (regex[0] == '~') {
    for (int i = 0; i <= restriction && i <= (int) strlen(word); i++) {
      if (match(regex + 1, word + i, restriction)) {
        return 1;
      }
    }
  } else {
    if (word[0] == regex[0]) {
      return match(regex + 1, word + 1, restriction);
    } else {
      return 0;
    }
  }
  return 0;
}
