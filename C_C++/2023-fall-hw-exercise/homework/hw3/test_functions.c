//Bella Lu xlu62

#include <assert.h>
#include <stdio.h>

#include "functions.h"

#define TEST_RESTRICTION_1 10
#define TEST_RESTRICTION_2 2
#define TEST_RESTRICTION_3 5
#define TEST_RESTRICTION_4 30

#define MAX_WORD_NUM 1024

void test_read_files() {
  FILE* fptr = fopen("test_file_1.txt", "w");
  fprintf(fptr, "5\nintermediate\ninttterrmeediateeee\nprogram\nppproograaamm\naaa");
  fclose(fptr);
}

void test_match_regex() {
  assert(match("abc", "abc", TEST_RESTRICTION_1) == 1);
  assert(match("abcd", "abc", TEST_RESTRICTION_1) == 0);
  assert(match("", "", TEST_RESTRICTION_1) == 1);

  // TODO: Write your own tests!
  assert(match("abc", "ab", TEST_RESTRICTION_1) == 0);
  assert(match("abcdefghijklm", "abcdefghijklm", TEST_RESTRICTION_1) == 1);
  assert(match("effort", "effort", TEST_RESTRICTION_1) == 1);
  assert(match("1923@#$%!&", "1923@#$%!&", TEST_RESTRICTION_1) == 1);
  assert(match("2@6#3$1%!0&", "2@6#3$%!0&", TEST_RESTRICTION_1) == 0);
  assert(match("AbCde", "AbCde", TEST_RESTRICTION_1) == 1);
  assert(match("abcde", "AbCde", TEST_RESTRICTION_1) == 0);
  assert(match("aaa", "aaa", TEST_RESTRICTION_1) == 1);

}

void test_match_regex_star() {
  assert(match("abcd*", "abc", TEST_RESTRICTION_1) == 1);
  assert(match("abc*d", "abcccccccd", TEST_RESTRICTION_1) == 1);
  assert(match("abc*de", "abcccccccd", TEST_RESTRICTION_1) == 0);

  // TODO: Write your own tests!
  assert(match("b*an", "bbbbbbbbban", TEST_RESTRICTION_1) == 1);
  assert(match("b*aned", "bbbbbbbbbanedd", TEST_RESTRICTION_1) == 0);
  assert(match("b*anedd", "bbbbbbbbbaned", TEST_RESTRICTION_1) == 0);
  assert(match("ban*", "bannnnnnnnnnnnnnnnnnnn", TEST_RESTRICTION_1) == 1);
  assert(match("*abc", "abc", TEST_RESTRICTION_1) == 0);
  assert(match("ab**cd", "abcd", TEST_RESTRICTION_1) == 0);
  assert(match("a*bc", "bc", TEST_RESTRICTION_1) == 1);
  assert(match("a*b*c*d*", "abcd", TEST_RESTRICTION_1) == 1);
  assert(match("a*b*c*1*", "1", TEST_RESTRICTION_1) == 1);
  assert(match("a*b*c*d*", "o", TEST_RESTRICTION_1) == 0);
}

void test_match_regex_question() {
  assert(match("a?", "", TEST_RESTRICTION_1) == 1);
  assert(match("a?", "b", TEST_RESTRICTION_1) == 0);
  assert(match("a?", "a", TEST_RESTRICTION_1) == 1);

  // TODO: Write your own tests!
  assert(match("a?a?a?", "", TEST_RESTRICTION_1) == 1);
  assert(match("ambd?g?uity", "ambiguity", TEST_RESTRICTION_1) == 0);
  assert(match("ambid?g?uity", "ambiguity", TEST_RESTRICTION_1) == 1);
  assert(match("voi??e", "voice", TEST_RESTRICTION_1) == 0);
  assert(match("?hi", "hi", TEST_RESTRICTION_1) == 0);
  assert(match("bi?", "big", TEST_RESTRICTION_1) == 0);
}

void test_match_regex_tilde() {
  assert(match("~", "aaaa", TEST_RESTRICTION_1) == 1);
  assert(match("~", "", TEST_RESTRICTION_1) == 1);
  assert(match("~", "a", TEST_RESTRICTION_1) == 1);

  // TODO: Write your own tests!
  assert(match("~~", "aaaaaaaaaaaaaa", TEST_RESTRICTION_1) == 1);
  assert(match("~~", "aaaaaaaaaaaaaaaaaaaaaaaaa", TEST_RESTRICTION_1) == 0);
  assert(match("~aa", "bananaaa", TEST_RESTRICTION_1) == 1);
  assert(match("~ab~", "banabnaaa", TEST_RESTRICTION_1) == 1);
  assert(match("a~", "abbbbb", TEST_RESTRICTION_1) == 1);
  assert(match("~abcd", "bcd", TEST_RESTRICTION_1) == 0);
  assert(match("~abcd", "abcd", TEST_RESTRICTION_1) == 1);
}

void test_match_regex_multiple() {
  assert(match("!2*keK?ee?rCC?C*aP?E*eR*T*", "!2222keKerCCCaeRRRT",
               TEST_RESTRICTION_1) == 1);
  assert(match("!~2*keK?ee?rCC?C*aP?E*eR*T*", "!2222keKerCCCaeRRRT",
               TEST_RESTRICTION_1) == 1);
  assert(match("~abc", "sda12!$2", TEST_RESTRICTION_1) == 0);

  // TODO: Write your own tests!
  assert(match("~ab*c~", "1$#@abbbbbcASCT", TEST_RESTRICTION_3) == 1);
  assert(match("~~a?b*c~~", "1$#@bbbbbcASCT", TEST_RESTRICTION_2) == 1);
  assert(match("~~a?b*c~~", "1$#@0bbbbbcASCT", TEST_RESTRICTION_2) == 0);
  assert(match("A?b?c*d*e*@?#*3?", "", TEST_RESTRICTION_1) == 1);
  assert(match("~a~b~c~d~e*f?", "abcdf", TEST_RESTRICTION_4) == 1);
  assert(match("a?*b*c", "abc", TEST_RESTRICTION_4) == 0);
  assert(match("a*~c?", "abc", TEST_RESTRICTION_4) == 1);
}

void test_match_regex_tilde_restriction() {
  // TODO: Write your own tests!
  assert(match("~~", "aaaaaaaaaaaaaa", TEST_RESTRICTION_2) == 0);
  assert(match("~~", "aaaaaaaaaaaaaaaaaaaaaaaaa", TEST_RESTRICTION_3) == 0);
  assert(match("~~b~ig", "aabbbig", TEST_RESTRICTION_2) == 1);
  assert(match("~small", "notAtAllsmall", TEST_RESTRICTION_3) == 0);
  assert(match("a~b", "aaab", TEST_RESTRICTION_2) == 1);
  assert(match("a~b", "accccb", TEST_RESTRICTION_2) == 0);
  assert(match("a~b", "aaac", TEST_RESTRICTION_2) == 0);
  assert(match("~abc", "aaaaaabc", TEST_RESTRICTION_3) == 1);
  assert(match("a~b~c", "anbnc", TEST_RESTRICTION_3) == 1);
  assert(match("~ab~", "nnabnn", TEST_RESTRICTION_2) == 1);
  assert(match("~", "kjdslkfjadsl", TEST_RESTRICTION_3) == 0);
  assert(match("~", "7df0^@f8dfm", TEST_RESTRICTION_3) == 0);
  assert(match("~", "yg7", TEST_RESTRICTION_3) == 1);
}

int main() {
  printf("Starting Tests...\n");
  test_read_files();
  test_match_regex();
  test_match_regex_star();
  test_match_regex_question();
  test_match_regex_tilde();
  test_match_regex_multiple();
  test_match_regex_tilde_restriction();
  printf("All tests passed!!!\n");
}
