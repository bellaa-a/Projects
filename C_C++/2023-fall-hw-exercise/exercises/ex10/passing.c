#include <stdio.h>

// TODO: declare the getDate function
int getDate (int *mon, int *day, int *yr);

int main() {

  int mon = 0;
  int day = 0;
  int yr = 0;

  // TODO: declare and initialize an array of strings that are the names of the
  // months of the year. You can use their 3-character abbreviations, such as "Feb".
  char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  
  while (getDate(&mon, &day, &yr) == 3) {
    printf("%s %d, %d\n", months[mon-1], day, yr);
  }
  
  return 0;
}

// TODO: define the getDate function to prompt for and read a date in the form
// MM/DD/YYYY, passing the inputs as integers back to main through the parameters
// and returning the number of values successfully read
int getDate (int *mon, int *day, int *yr){
  int m = 0;
  int d = 0;
  int y = 0;
  printf("enter date in MM/DD/YYYY format: ");
  int inputs = scanf(" %d/%d/%d", &m, &d, &y);
  *mon = m;
  *day = d;
  *yr = y;
  return inputs;
}

