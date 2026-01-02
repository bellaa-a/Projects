#include <stdio.h>
#include <string.h>
#include "string_functions.h"

int concat(const char word1[], const char word2[], char result[], int result_capacity){

   //TODO: replace the following stub with a correct function body so that 
   //      this function behaves as indicated in the comment above
   //
   //NOTE: you may not use the strcat or strcpy library functions in your solution!
   int len1 = strlen(word1);
   int len2 = strlen(word2);
   if ((len1 + len2) > result_capacity -1){
      return 1;
   } else {
      for (int i = 0; i < len1; i++) {
         result[i] = word1[i];
      }
      int j = 0;
      for (int i = len1; i < (len1 + len2); i++) {
         result[i] = word2[j];
         j++;
      }
      result[len1 + len2] = '\0';
      return 0;
   }

   return -1;

}