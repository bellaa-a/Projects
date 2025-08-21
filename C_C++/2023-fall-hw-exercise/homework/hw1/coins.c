//Hopkins ID: 922C62

#include <stdio.h>
#include <ctype.h>

int main(){

  int denominations = 0;
  char inputId;
  char command;
  int num = 0;
  char identifier;
  int value;
  
  printf("How many denominations? ");
  scanf(" %d", &denominations);

  char identifiers[denominations];
  int values[denominations];
  int freq[denominations];

  for (int i=0; i<denominations; i++){
    values[i]=0;
    freq[i]=0;
    identifiers[i]='\0';
  }

  if (denominations<1 || denominations>20){
    fprintf(stderr, "Invalid input\n");
    return 1;
  }

  for (int i=0; i<denominations; i++){
    printf("Enter coin identifier and value in cents: ");
    int returnValue = scanf(" %c %d", &identifier, &value);
    if (!isalpha(identifier) || returnValue !=2){
      fprintf(stderr, "Invalid input\n");
      return 1;
    }
    identifiers[i]=identifier;
    values[i]=value;
  }
  double  sum = 0.00;
  printf("Enter a command: ");

  while (scanf(" %c", &command)!=EOF){
    if (command == 'a'){
      int returnValue2 = scanf(" %c %d", &inputId, &num);
      if (!isalpha(inputId) || returnValue2 !=2){
        fprintf(stderr, "Unknown coin identifier\n");
        return 2;
      }
      for (int i=0; i<denominations; i++){
	      if (identifiers[i]==inputId){
	        freq[i]+=num;
	      }
      }
    } else if (command == 'r'){
        int returnValue3 = scanf(" %c %d", &inputId, &num);
        if (!isalpha(inputId) || returnValue3 !=2){
          fprintf(stderr, "Unknown coin identifier\n");
          return 2;
        }
        for (int i=0; i<denominations; i++){
	        if (identifiers[i]==inputId){
	          freq[i]-=num;
	        }
        } 
    } else if (command == 's'){
      printf("Identifier,Face Value,Count,Total Value\n");
      for (int i=0; i<denominations; i++){
        printf(" %c,%d,%d,%d\n", identifiers[i], values[i], freq[i], values[i]*freq[i]);
        sum += (double)values[i]/100*freq[i];
      }
	printf("Overall value of collection: $%0.2f\n", sum);
  sum = 0.00;
      } else if (command == 'q'){
	break;
      } else {
        fprintf(stderr, "Invalid command\n");
        return 3;
      }
  printf("Enter a command: ");
  }
  printf("Bye!\n");
  return 0;
}