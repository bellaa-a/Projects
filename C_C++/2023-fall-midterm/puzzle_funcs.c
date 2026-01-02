#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "puzzle.h"
#include "ppm_io.h"

Puzzle * puzzle_create(int size){
  //create a new puzzle, initializing its size and 2D array 
  Puzzle * newPuz = (Puzzle *) malloc(sizeof(Puzzle));
  newPuz->size = size; 
  newPuz->puzzle2D = malloc(sizeof(int*)*size);
  int count = 1; 
  for(int i = 0; i < size; i++){
      newPuz->puzzle2D[i] = calloc(size, sizeof(int)*size); 
  }

  //initialize puzzle2D into a ordered puzzle 
  for(int row = 0; row < size; row++){
    for(int col = 0; col < size; col++){
      newPuz->puzzle2D[row][col] = count;
      count++;
      if(row == size - 1 && col == size - 1){
        newPuz->puzzle2D[row][col] = 0; 
      }
    }
  }
  return newPuz; 
}

void puzzle_destroy(Puzzle *p, int executedC){
  //check executed C to see if puzzle2D was allocated 
  if(executedC == 1){
    for (int i = 0; i < p->size; i++) {
      free (p->puzzle2D[i]);
    }
    free(p->puzzle2D);
    free(p); 
  }
  else free(p);
  
}

void image_destroy(Image * backGround, int executedI){
  //check executed I to see if data within the Image struct was allocated 
  if(executedI == 1){
    free (backGround->data);
    free (backGround);
  }
  else free(backGround);
}

void puzzle_set_tile(Puzzle *p, int col, int row, int value){
  p->puzzle2D[row][col] = value;
}

int puzzle_get_tile(const Puzzle *p, int col, int row){
  return p->puzzle2D[row][col];
}

int handle_C_command(FILE *in, Puzzle **p){
  int size = 0; 
  int numInputs = fscanf(in, " %d", &size);

  if (numInputs != 1) {
    fprintf(stderr, "Invalid input\n");
    return 0; 
  }

  if (size > 20 || size < 2){
    fprintf(stderr, "Invalid puzzle size\n");
    return 0; 
  }

  if(p){
    free(*p);
  }

  *p = puzzle_create(size);   
  return 1;    
}

int handle_T_command(FILE * in, Puzzle *p){
  char command = '\0';
  int order = 0; 
  int count = 0; 
  int temp[p->size*p->size]; 


  for(int r = 0; r < p->size; r++){
    for (int c = 0; c < p->size; c++){
      int numInputs = fscanf(in, " %d", &order); 

      //check whether there are multiple inputs 
      if (numInputs != 1) {
        fprintf(stderr, "Invalid input\n");
        return 0; 
      }

      //initialize the location of the empty tile 
      if(order == 0){
        p->spaceCol = c;
        p->spaceRow = r;        
      }
      
      //check whether the order is within the boundary 
      if (order < 0 || order > p->size*p->size){
        fprintf(stderr, "Invalid tile value\n");
        return 0; 
      }
      //insert order into puzzle2D
      p->puzzle2D[r][c] = order; 
    }
  }

  //place values from puzzle2D into temp, a 1D array
  for (int i = 0; i < p->size; i ++){
    for (int j = 0; j < p->size; j++){
      temp[count] = p->puzzle2D[i][j];
      count ++; 
    }
  }

  //check whether there are repetitive tiles 
  for(int i = 0; i < p->size*p->size - 1; i++){
    for (int j = i + 1; j < p->size*p->size; j++){
      if (temp[i] == temp[j]){
        fprintf(stderr, "Invalid tile value\n");
        return 0; 
      }
    }
  }
  return 1; 
}

//puzzle moves the blank tile and updates its postion 
void puzzleMove(Puzzle *p, int targetRow, int targetCol){
  p->puzzle2D[p->spaceRow][p->spaceCol] = p->puzzle2D[targetRow][targetCol]; 
  p->puzzle2D[targetRow][targetCol] = 0; 
  p->spaceRow = targetRow;
  p->spaceCol = targetCol;
}

//handle S changes the order of the tile based on the u d l r commands using puzzleMove
int handle_S_command(FILE *in, Puzzle **p) {
  char command = '\0';
  int numInputs = fscanf(in, " %c", &command); 
  //error handling: check whether S is followed by a valid command 
  if (numInputs != 1) {
    fprintf(stderr, "Invalid input\n");
    return 0; 
  }

  int spaceRow = (*p)->spaceRow; 
  int spaceCol = (*p)->spaceCol; 

  switch(command){
    case 'u':
      //check whether the move is within the boundary of the puzzle 
      if(spaceRow+1 >= (*p)->size){
        fprintf(stderr, "Puzzle cannot be moved in specified direction\n");
        return 0; 
      }
      puzzleMove(*p, spaceRow+1, spaceCol); 
      break;
    
    case 'd':
      if (spaceRow-1 < 0){
        fprintf(stderr, "Puzzle cannot be moved in specified direction\n");
        return 0; 
      }
      puzzleMove(*p, spaceRow-1, spaceCol); 
      break;

    case 'l':
      if(spaceCol+1 >= (*p)->size){
        fprintf(stderr, "Puzzle cannot be moved in specified direction\n");
        return 0; 
      }
      puzzleMove(*p, spaceRow, spaceCol+1); 
      break; 
    
    case 'r':
      if(spaceCol-1 < 0){
        fprintf(stderr, "Puzzle cannot be moved in specified direction\n");
        return 0; 
      }
      puzzleMove(*p, spaceRow, spaceCol-1); 
      break;
    
    default: 
      fprintf(stderr, "Invalid input\n");
      return 0; 
  }

  return 1; 
    
}

int is_solved(Puzzle *p) {  
  int count = 1; 
  for(int r = 0; r < p->size; r++){
    for(int c = 0; c < p->size; c++){
      //check whether the last tile is empty 
      if (r == p->size-1 && c == p->size-1 && p->puzzle2D[r][c] == 0){
        return 1; 
      }
      //check whether the puzzle is in the correct order 
      if (p->puzzle2D[r][c] != count++){
        return 0; 
      }
    }  
  }
  return 0; 
}

//sets the possible directions to be tested in solve puzzle 
//The logic is that the possible direction should not reverse 
//the previous step taken to solve puzzle
void findDirections(char *direction, char preStep){
  switch (preStep)
  {
  case 'u':
    direction[0] = 'u';
    direction[1] = 'l';
    direction[2] = 'r';
    direction[3] = '\0';
    break;
  case 'd':
    direction[0] = 'd';
    direction[1] = 'l';
    direction[2] = 'r';
    direction[3] = '\0';
    break;
  case 'l':
    direction[0] = 'u';
    direction[1] = 'l';
    direction[2] = 'd';
    direction[3] = '\0';
    break;
  case 'r':
    direction[0] = 'd';
    direction[1] = 'u';
    direction[2] = 'r';
    direction[3] = '\0';
    break;
  default:
    direction[0] = 'd';
    direction[1] = 'u';
    direction[2] = 'r';
    direction[3] = 'l';
    break;
  }
}

int solve_puzzle(Puzzle *p, char steps[], int max_steps, int cur_steps, char preStep) {
  //The char array, direction, holds the next three possible directions 
  //because it doesn't make sense if you move space in the opposite direction 
  //in which we just moved it. 
  char direction[4] = {'d', 'u', 'r', 'l'};

  //dirIndex is index of the direction array, which increments in order to 
  //check all possible directions. 
  int dirIndx = 0;

  //findDirections finds the next three possible directions based on the previous 
  //step (preStep), and stores it in an array (direction).
  findDirections(direction, preStep);

  //Terminates if the puzzle is solved.
  if (is_solved(p) == 1){
    return 1; 
  }

  //Terminates if the amount of steps tried exceeds the maxminum number of 
  //steps it was told to try, in other words, we failed to find a solution
  //within the number of maximum number of steps. 
  if (cur_steps >= max_steps){
    return 0;
  }

  while(dirIndx < 4){
    if(direction[dirIndx] == 'u'){
       //Check if space can be moved in specified direction.
      if(p->spaceRow + 1 < (p)->size) {
        //Tries to move it. 
        puzzleMove(p, p->spaceRow + 1, p->spaceCol);
        //Updates steps array, which contains the final directions to solve the 
        //puzzle, if a solution is found.
        steps[cur_steps] = 'u';
        //Check if step taken if feasible. 
        if (solve_puzzle(p, steps, max_steps, cur_steps + 1, 'u') != 0) return 1;
        //If not feasible then reverses move. 
        else puzzleMove(p, p->spaceRow-1, p->spaceCol); 
      }    
    }

    else if(direction[dirIndx] == 'd'){
      if(p->spaceRow -1 >= 0){
        puzzleMove(p, p->spaceRow - 1, p->spaceCol);
        steps[cur_steps] = 'd';
        if (solve_puzzle(p, steps, max_steps, cur_steps + 1, 'd') != 0)
          return 1;
        else puzzleMove(p, p->spaceRow +1, p->spaceCol); 
      } 
    }

    else if(direction[dirIndx] == 'l'){
      if(p->spaceCol+1 < (p)->size){
        puzzleMove(p, p->spaceRow, p->spaceCol+1); 
        steps[cur_steps] = 'l'; 
          if (solve_puzzle(p, steps, max_steps, cur_steps + 1, 'l') != 0)
            return 1;
          else
            puzzleMove(p, p->spaceRow, p->spaceCol - 1); 
      }
    }

    else if(direction[dirIndx] == 'r'){
      if(p->spaceCol-1 >= 0){
        puzzleMove(p, p->spaceRow, p->spaceCol-1); 
        steps[cur_steps] = 'r'; 
        if (solve_puzzle(p, steps, max_steps, cur_steps + 1, 'r') != 0) return 1;
        else puzzleMove(p, p->spaceRow, p->spaceCol+1); 
      } 
    }

    else{
      return 0; 
    }
    dirIndx++; 
  }
}

void handle_V_command(Puzzle *p, char steps[]){
    int found = 0;
    int count = 0; 
    for (int i = 0; i < 1024; i++){
      //test set amount of possible solutions 
      if (solve_puzzle(p, steps, i, 0, 'z') == 1){
        found = 1;
        count++; 
        break;
      }
      else count++; 
      
    }
    if (found == 0) {
      printf("No solution found\n");
    }   

    else {
      //in case when the puzzle is already solved 
      if(count == 1){
          return; 
      }

      for (int i = 0; i < count - 1; i++) {
          printf("S %c\n", steps[i]);
      }
    }
}

void handle_K_command(Puzzle * p){
  if(is_solved(p)){
    printf("Solved\n");
  } 
  else {
    printf("Not solved\n");
  }
}

int handle_I_command(FILE * in, Image ** backGround){

  char * picPtr = malloc(sizeof(char)*255); 

  //error handling: check whether there is a file name following the I command
  if(fscanf(in, " %s", picPtr) != 1){
    fprintf(stderr, "Invalid input\n");
    free(picPtr); 
    return 0; 
  }

  FILE * backGroundImage = fopen(picPtr, "r"); 
  //error handling: check whether the image file exists 
  if(backGroundImage == NULL){
    fprintf(stderr, "Could not open image file '%s'\n", picPtr);
    free(picPtr);
    return 0; 
  }

  free(* backGround);
  *backGround = ReadPPM(backGroundImage); 
  free(picPtr);
  fclose(backGroundImage); 
  return 1; 
}

//a helper function for handle W 
int startVal (int order, int colSizeArr, int colImage){
  int rVal = 0; 
  int cVol = order - 1; 

  if(order > colSizeArr){
    for(int i = order; i > colSizeArr; i-= colSizeArr){
      rVal++; 
    }
    order -= rVal*colSizeArr; 
    cVol = order - 1; 
  }
  return cVol*colImage/colSizeArr + rVal*colImage*colImage/colSizeArr;
}

int handle_W_command(Image * backGround, FILE * in, Puzzle * p){

  char * resultPPM = malloc(sizeof(char)*225); 
  char * resultTXT = malloc(sizeof(char)*225); 

  //Reads the two inputs after 'W', one ppm file and one text file.
  //If fails, free the two malloc'ed char arrays. 
  if (fscanf(in," %s %s", resultPPM, resultTXT) != 2){
    fprintf(stderr, "Invalid input\n");
    free(resultPPM); 
    free(resultTXT); 
    return 0; 
  } 
  
  //Checks if dimentions of given image is divisible into the number of specified
  //tile given by the command 'c'.
  if(backGround->rows != backGround->cols || backGround->rows % p->size != 0){
    fprintf(stderr, "Invalid image dimensions\n");
    free(resultPPM); 
    free(resultTXT);
    return 0; 
  }

  FILE * filePPM = fopen(resultPPM, "w"); 
  FILE * fileTXT = fopen(resultTXT, "w"); 

  if(filePPM == NULL){
    fprintf(stderr, "Could not open output image file '%s'\n", resultPPM);
    free(resultPPM); 
    free(resultTXT);
    return 0; 
  }
  else if (fileTXT == NULL){
    fprintf(stderr, "Could not open output image file '%s'\n", resultTXT);
    free(resultPPM); 
    free(resultTXT);
    return 0; 
  }

  //Creates a temperary image that contains the background so we can manipulate it. 
  Image * temp = malloc(sizeof(Image)); 
  temp->data = malloc(sizeof(backGround->data)*backGround->rows*backGround->cols); 
  temp->rows = backGround->rows; 
  temp->cols = backGround->cols; 

  //Total number of pixles in each puzzle block. 
  int segmentRow = (backGround->rows*backGround->cols)/(p->size);
  //The number of pixles for one dimention of each puzzle block. 
  int segmentBlock = (backGround->rows)/(p->size);
  //The number of pixles jumped in between 
  int jumpVal = backGround->rows / p->size * (p->size - 1); 
  int countBlock = 1;
  int countPixel = 0; 

  for(int puzR = 0; puzR < p->size; puzR++){
    for(int puzC = 0; puzC < p->size; puzC++){
      fprintf(fileTXT, "%d ", p->puzzle2D[puzR][puzC]);
      int startValBackg = startVal (p->puzzle2D[puzR][puzC], p->size, backGround->cols); 
      int startValTemp = startVal (countBlock, p->size, backGround->cols); 

      for(int i = startValTemp; i < segmentRow*(puzR+1); i+=jumpVal){
        
        
        if(p->puzzle2D[puzR][puzC] == 0){
          for(int dataIndx = 0; dataIndx < segmentBlock; dataIndx++){
            temp->data[i].r = '\0';
            temp->data[i].g = '\0';
            temp->data[i].b = '\0';
            i++; 
          }
        }
        else {
          for(int dataIndx = 0; dataIndx < segmentBlock; dataIndx++){
            temp->data[i] = backGround->data[startValBackg + countPixel];
            countPixel++; 
            i++; 
          }
        }
        countPixel+=jumpVal; 
      }
      countBlock++; 
      countPixel = 0; 
    }
  }
  if(WritePPM(filePPM, temp) == -1){
    fprintf(stderr, "Could not write puzzle data %s\n", resultPPM);
    free(resultPPM); 
    free(resultTXT);
    fclose(filePPM); 
    fclose(fileTXT); 
    free(temp->data);
    free(temp);
    return 0; 
  }

  fclose(filePPM); 
  fclose(fileTXT); 
  free(resultPPM); 
  free(resultTXT); 
  free(temp->data);
  free(temp);
  return 1; 
}

void handle_P_command(Puzzle *p){
  for (int i = 0; i < p->size; i ++){
    for (int j = 0; j < p->size; j++){
      printf("%d ", p->puzzle2D[i][j]); 
    }
  }
  printf("\n"); 
}