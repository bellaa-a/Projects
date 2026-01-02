#ifndef PUZZLE_H
#define PUZZLE_H

#include "ppm_io.h" // for Image data type

typedef struct {
  int size;
  int **puzzle2D;  
  int spaceRow;
  int spaceCol; 
} Puzzle;

// TODO: function declarations
Puzzle * puzzle_create(int size);
void puzzle_destroy(Puzzle *p, int executedC); 
void image_destroy(Image * backGround, int executedI);
void puzzle_set_tile(Puzzle *p, int col, int row, int value);
int puzzle_get_tile(const Puzzle *p, int col, int row);
int handle_S_command(FILE *in, Puzzle **p);
int handle_C_command(FILE *in, Puzzle **p);
int handle_T_command(FILE * in, Puzzle *p);
int is_solved(Puzzle *p); 
void findDirections(char *direction, char preStep);
int solve_puzzle(Puzzle *p, char steps[], int max_steps, int cur_steps, char preStep);
void handle_V_command(Puzzle *p, char steps[]);
int handle_I_command(FILE * in, Image ** backGround);
int handle_W_command(Image * backGround, FILE * in, Puzzle * p);
void handle_P_command(Puzzle *p); 
void handle_K_command(Puzzle * p);
void puzzleMove(Puzzle *p, int targetRow, int targetCol);
int startVal (int order, int colSizeArr, int colImage);
//abcdef 

#endif // PUZZLE_H