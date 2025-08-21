#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ppm_io.h"
#include "puzzle.h"

int main(int argc, char **argv) {

  char command  = '\0';
  char steps[1024];
  int executedC = 0; 
  int executedI = 0; 

  if (argc < 1 || argc > 2) {
    fprintf(stderr, "Usage: ./puzzle [<command file>]\n");
    return 1;
  }

  Puzzle * newPuz = malloc(sizeof(Puzzle));
  Image * backGround = malloc(sizeof(Image)); 
  
  //if there is a test file 
  if (argc == 2) {
    const char * charPtr = argv[1]; 
    FILE * in = fopen(charPtr, "r"); 
    
    while(fscanf(in, " %c", &command) == 1){
      
      if(command == 'C'){
        //check whether the command has been successfully executed, if not 
        //exit program and free all the allocated memory 
        if(handle_C_command(in, &newPuz)==0) {
          fclose(in); 
          puzzle_destroy(newPuz, executedC); 
          image_destroy(backGround, executedI); 
          return 1; 
          } 
          executedC = 1; 
        }

      else if (command == 'T'){
        //check whether the puzzle have been initialized 
        if (executedC == 0){
          fprintf(stderr, "No puzzle\n");
          fclose(in); 
          puzzle_destroy(newPuz, executedC); 
          image_destroy(backGround, executedI);
          return 1; 
        }

        if(handle_T_command(in, newPuz) == 0){
          fclose(in); 
          puzzle_destroy(newPuz, executedC); 
          image_destroy(backGround, executedI);
          return 1; 
        } 
      }

      else if(command == 'S'){
        if(handle_S_command(in, &newPuz) == 0){
          fclose(in); 
          puzzle_destroy(newPuz, executedC); 
          image_destroy(backGround, executedI);
          return 1; 
        }
      } 
      
      else if(command == 'I'){
        if(handle_I_command(in, &backGround) == 0){
          fclose(in); 
          puzzle_destroy(newPuz, executedC); 
          image_destroy(backGround, executedI);
          return 1; 
        } 
        executedI = 1; 
      } 
      
      else if(command == 'W'){
        if (executedC == 0){
          fprintf(stderr, "No puzzle\n");
          fclose(in); 
          puzzle_destroy(newPuz, executedC); 
          image_destroy(backGround, executedI);
          return 1; 
        }
        else if(executedI == 0){
          fprintf(stderr, "No image\n");
          fclose(in); 
          puzzle_destroy(newPuz, executedC); 
          image_destroy(backGround, executedI);
          return 1; 
        }

        if (handle_W_command(backGround, in, newPuz) == 0){
          fclose(in); 
          puzzle_destroy(newPuz, executedC); 
          image_destroy(backGround, executedI);
          return 1; 
        } 
      } 
      
      else if (command == 'V'){
        if (executedC == 0){
          fclose(in); 
          fprintf(stderr, "No puzzle\n");
          puzzle_destroy(newPuz, executedC); 
          image_destroy(backGround, executedI);
          return 1; 
        }
        handle_V_command(newPuz, steps); 
      }
      
      else if(command == 'P'){
        if (executedC == 0){
          fclose(in); 
          fprintf(stderr, "No puzzle\n");
          puzzle_destroy(newPuz, executedC); 
          image_destroy(backGround, executedI);
          return 1; 
        }
        handle_P_command(newPuz); 
      }
      
      else if(command == 'K'){
        
        if (executedC == 0){
          fclose(in); 
          fprintf(stderr, "No puzzle\n");
          puzzle_destroy(newPuz, executedC); 
          image_destroy(backGround, executedI);
          return 1; 
        }
        handle_K_command(newPuz);
      }
      
      else if (command == 'Q') {
        fclose(in);
        puzzle_destroy(newPuz, executedC); 
        image_destroy(backGround, executedI); 
        return 0;
      } 
      else {
        fprintf(stderr, "Invalid command '%c'\n", command);
        fclose(in); 
        puzzle_destroy(newPuz, executedC); 
        image_destroy(backGround, executedI); 
        return 1;
      }                        
    }
    puzzle_destroy(newPuz, executedC); 
    image_destroy(backGround, executedI);
    fclose(in);
  }

  // if we are relying on user input 
  else {
    while(fscanf(stdin, " %c", &command) == 1){
      if(command == 'C'){
        //the code creates a temporary file with neccessary input after the command 
        //to be passed into the functions 
        FILE * in = fopen("input.txt", "w"); 
        int size = 0;
        fscanf(stdin, " %d", &size);
        fprintf(in, " %d", size);
        fclose(in);

        in = fopen("input.txt", "r");
        if(handle_C_command(in, &newPuz)== 0){
          fclose(in);
          puzzle_destroy(newPuz, executedC); 
          image_destroy(backGround, executedI); 
          return 1; 
        } 
        executedC = 1; 
        fclose(in);
      }

      else if (command == 'T'){

        if (executedC == 0){
          fprintf(stderr, "No puzzle\n");
          puzzle_destroy(newPuz, executedC); 
          image_destroy(backGround, executedI); 
          return 1; 
        }

         FILE * in = fopen("input.txt", "w"); 
        int inputVal = 0;
        for(int i = 0; i < newPuz->size*newPuz->size; i++) {
          fscanf(stdin, " %d", &inputVal); 
          fprintf(in, " %d", inputVal);
        }
        fclose(in); 

        in = fopen("input.txt", "r"); 
        if(handle_T_command(in, newPuz) == 0){
          puzzle_destroy(newPuz, executedC); 
          image_destroy(backGround, executedI); 
          fclose(in); 
          return 1;
        } 
        fclose(in); 
      }

      else if(command == 'S'){
        FILE * in = fopen("input.txt", "w"); 
        char move = '\0';
        fscanf(stdin, " %c", &move); 
        fprintf(in, " %c", move);

        fclose(in); 
        in = fopen("input.txt", "r"); 
        if(handle_S_command(in, &newPuz) == 0){
          fclose(in); 
          puzzle_destroy(newPuz, executedC); 
          image_destroy(backGround, executedI); 
          return 1; 
        } 
        fclose(in); 
      }

      else if(command == 'I'){
        FILE * in = fopen("input.txt", "w"); 
        char picPtr[255];
        if (fscanf(stdin, " %s", picPtr) == 1){
           fprintf(in, " %s", picPtr);
        }
        else{
           fprintf(stderr, "Invalid input\n");
           fclose(in); 
           puzzle_destroy(newPuz, executedC); 
           image_destroy(backGround, executedI); 
           return 1; 
        }
        fclose(in); 
        in = fopen("input.txt", "r");
        if(handle_I_command(in, &backGround) == 0){
          fclose(in); 
          puzzle_destroy(newPuz, executedC); 
          image_destroy(backGround, executedI); 
          return 1; 
        } 
        fclose(in); 
        executedI = 1; 
      } 
      
      else if(command == 'W'){
        if (executedC == 0){
          fprintf(stderr, "No puzzle\n");
          puzzle_destroy(newPuz, executedC); 
          image_destroy(backGround, executedI); 
          return 1; 
        }
        
        if(executedI == 0){
          puzzle_destroy(newPuz, executedC); 
          image_destroy(backGround, executedI); 
          fprintf(stderr, "No image\n");
          return 1; 
        }

        FILE * in = fopen("input.txt", "w"); 
        char resultPPM[225];
        char resultTXT[225];
        if(fscanf(stdin," %s %s", resultPPM, resultTXT) == 2){
          fprintf(in, " %s %s", resultPPM, resultTXT);

        }

        else {
          fprintf(stderr, "Invalid input\n");
          puzzle_destroy(newPuz, executedC); 
          image_destroy(backGround, executedI); 
          fclose(in);
          return 1; 
        }

        fclose(in);
        in = fopen("input.txt", "r");
        if (handle_W_command(backGround, in, newPuz) == 0){
          puzzle_destroy(newPuz, executedC); 
          image_destroy(backGround, executedI); 
          fclose(in); 
          return 1; 
        } 
        fclose(in); 
      } 
      
      else if (command == 'V'){
        if (executedC == 0){
          fprintf(stderr, "No puzzle\n");
          puzzle_destroy(newPuz, executedC); 
          image_destroy(backGround, executedI); 
          return 1; 
        }
        handle_V_command(newPuz, steps); 
      }
      
      else if(command == 'P'){
        if (executedC == 0){
          fprintf(stderr, "No puzzle\n");
          puzzle_destroy(newPuz, executedC); 
          image_destroy(backGround, executedI); 
          return 1; 
        }
        handle_P_command(newPuz); 
      }
      
      else if(command == 'K'){
        if (executedC == 0){
          fprintf(stderr, "No puzzle\n");
          puzzle_destroy(newPuz, executedC); 
          image_destroy(backGround, executedI); 
          return 1; 
        }
        handle_K_command(newPuz);
      }
      
      else if (command == 'Q') {
        puzzle_destroy(newPuz, executedC); 
        image_destroy(backGround, executedI); 
        return 0;
      } 
      else {
        fprintf(stderr, "Invalid command '%c'\n", command);
        puzzle_destroy(newPuz, executedC); 
        image_destroy(backGround, executedI); 
        return 1;
      }                        
    }
  }

puzzle_destroy(newPuz, executedC); 
image_destroy(backGround, executedI); 
  
 
  return 0; 
}