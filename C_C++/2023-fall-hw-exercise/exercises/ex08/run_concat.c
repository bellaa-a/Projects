#include <stdio.h>
#include <string.h>
#include "string_functions.h"

int main() {

  char word1[11];  // allow up to 10 chars, then room for '\0' terminator
  char word2[11];  // allow up to 10 chars, then room for '\0' terminator
  char result[18]; // allow up to 17 chars, then room for '\0' terminator
  int result_size = (int)sizeof(result)/sizeof(char); // number of elements in the result array

  //collect two strings
  printf("Enter the first word (up to 10 characters): ");
  scanf("%s", word1);
  printf("Enter the second word (up to 10 characters): ");
  scanf("%s", word2);

  //Call the concat function output the results
  int return_val = concat(word1, word2, result, result_size);
  printf("Called concat(\"%s\",\"%s\", result, %d)\n", word1, word2, result_size);
  printf("Return value was %d and result now equals \"%s\", with length %lu\n\n", 
     return_val, result, strlen(result));

  return 0;

}