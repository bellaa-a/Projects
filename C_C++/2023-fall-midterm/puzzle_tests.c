#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "puzzle.h"

// also tests puzzle_destroy
void test_puzzle_create(void) {
  int count = 1;
  Puzzle *p3 = puzzle_create(3);
  Puzzle *p5 = puzzle_create(5);

  assert(p3->size == 3);
  for (int r = 0; r < 3; ++r) {
    for (int c = 0; c < 3; ++c) {
      if(r == 2 && c == 2){
        assert(p3->puzzle2D[r][c] == 0); 
      }
      else assert(p3->puzzle2D[r][c] == count);
      count++;
    }
  }
  count = 1;
  assert(p5->size == 5);

   for (int r = 0; r < 5; ++r) {
    for (int c = 0; c < 5; ++c) {
      if(r == 4 && c == 4){
        assert(p5->puzzle2D[r][c] == 0); 
      }
      else assert(p5->puzzle2D[r][c] == count);
      count++;
    }
  }

  puzzle_destroy(p3, 1);
  puzzle_destroy(p5, 1);
}

// also tests puzzle_get_tile
void test_puzzle_set_tile(void) {
  int init[4][4] = {
    { 5, 7, 1, 2 },
    { 13, 9, 3, 4 },
    { 0, 8, 6, 11 },
    { 14, 15, 10, 12 },
  };

  Puzzle *p4 = puzzle_create(4);
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      puzzle_set_tile(p4, j, i, init[i][j]);

  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      assert(puzzle_get_tile(p4, j, i) == init[i][j]);

  puzzle_destroy(p4, 1);
}

// ...other test functions...
void test_puzzle_is_solved(void) {

  Puzzle *solvedP1a = puzzle_create(1);
  puzzle_set_tile(solvedP1a, 0, 0, 0);
  assert(is_solved(solvedP1a) == 1);

  Puzzle *solvedP1b = puzzle_create(1);
  puzzle_set_tile(solvedP1b, 0, 0, 1);
  assert(is_solved(solvedP1b) == 0);

  Puzzle *solvedP2 = puzzle_create(2);
  int init2[2][2] = {
    { 1, 2 },
    { 3, 0 },
  };
  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 2; ++j)
      puzzle_set_tile(solvedP2, j, i, init2[i][j]);
  assert(is_solved(solvedP2) == 1);

  Puzzle *scrambledP3 = puzzle_create(3);
  int init3[3][3] = {
    { 5, 7, 1 },
    { 2, 3, 4 },
    { 0, 8, 6 },
  };
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      puzzle_set_tile(scrambledP3, j, i, init3[i][j]);
  assert(is_solved(scrambledP3) == 0);

  Puzzle *solvedP4 = puzzle_create(4);
  int init4[4][4] = {
    { 1, 2, 3, 4 },
    { 5, 6, 7, 8 },
    { 9, 10, 11, 12 },
    { 13, 14, 15, 0 },
  };
  for (int i = 0; i < 4; i++){
    for (int j = 0; j < 4; j++){
      puzzle_set_tile(solvedP4, j, i, init4[i][j]);
    }
  }
    


  assert(is_solved(solvedP4) == 1);

  puzzle_destroy(solvedP1a, 1);
  puzzle_destroy(solvedP1b, 1);
  puzzle_destroy(solvedP2, 1);
  puzzle_destroy(scrambledP3, 1);
  puzzle_destroy(solvedP4, 1);
}

void test_puzzle_move(void) {
  Puzzle *p3 = puzzle_create(3);
  int init3[3][3] = {
    { 5, 7, 3 },
    { 2, 0, 4 },
    { 1, 8, 6 },
  };
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      puzzle_set_tile(p3, j, i, init3[i][j]);

  puzzleMove(p3, 2, 1);
  assert (p3->puzzle2D[2][1] == 0); 

  puzzleMove(p3, 2, 0);
  assert (p3->puzzle2D[2][0] == 0); 

  puzzle_destroy(p3, 1);
}

void test_puzzle_directions(void) { 
  char directions[4] = {'\0', '\0', '\0', '\0'};
  findDirections(directions, 'u');
  assert(directions[0] == 'u');
  assert(directions[1] == 'l');
  assert(directions[3] == '\0');

  findDirections(directions, 'r');
  assert(directions[0] == 'd');
  assert(directions[2] == 'r');
  assert(directions[3] == '\0');
}

void test_puzzle_starting_value(void) {
  Puzzle *p3 = puzzle_create(3);
  int init3[3][3] = {
    { 5, 7, 3 },
    { 2, 0, 4 },
    { 1, 8, 6 },
  };
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      puzzle_set_tile(p3, j, i, init3[i][j]);

  assert (startVal (p3->puzzle2D[0][0], 3, 414) == 57270);
  assert (startVal (p3->puzzle2D[2][0], 3, 414) == 0);
  assert (startVal (p3->puzzle2D[2][2], 3, 414) == 57408);

  puzzle_destroy(p3, 1);
}

int main(void) {

  test_puzzle_create();
  test_puzzle_set_tile();

  // ...call other test functions...
  
  test_puzzle_is_solved();
  test_puzzle_move();
  test_puzzle_directions();
  test_puzzle_starting_value();

  printf("All tests passed!\n");
  return 0;
}
