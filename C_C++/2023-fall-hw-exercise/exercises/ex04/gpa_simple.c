//Ex04: gpa_simple.c

/* The purpose of this program is to compute GPAs for simple letter
   grades - no +/-, only A, B, C, D, F. Credits may be rational
   numbers.  Also determine and display notices for Dean's List
   (>=3.5) and Academic Probation (< 2.0).

SAMPLE RUN:

Welcome to the GPA calculator!
Enter grade and credits for each course below (ctrl-d to end):
course 1: A 4.0
course 2: b 2.7
course 3: B 3.5
course 4: c 3.0
course 5: f 1
course 6: a 3
course 7: 
Your GPA is 3.06

-----
PSEUDOCODE:

point_sum gets 0
credit_sum gets 0
points gets 0
gpa gets 0

display “Welcome to GPA calculator!”
prompt for list of grade/credits pairs

count gets 1
display "course #", count

repeat while there is a grade and credits to read
    convert grade to points
    add points * credits to point_sum
    add credits to credit_sum
    add 1 to count
    display "course #", count

if credit_sum > 0
   set gpa to point_sum / credit_sum
   display "GPA is ", gpa
   if gpa >= 3.5
      display "Dean's List"
   otherwise if gpa <= 2.0
      display "Uh-oh, Academic Probation..."
otherwise
   display "No credits attempted; no GPA to report"

*/

#include <stdio.h>

int main() {

  //TO DO: add your code here so that your program                                                                                                                                                          
  //produces output that would be identical to the                                                                                                                                                          
  //given sample run when provided the same input                                                                                                                                                           
  int count = 1;
  char grade;
  double points = 0.00;
  double credits = 0.00;
  double point_sum = 0.00;
  double credit_sum = 0.00;
  double gpa = 0.00;
  printf("Welcome to the GPA calculator! \nEnter grade and credits for each course below (ctrl-d to end):\ncourse 1: ");
  while (scanf(" %c", &grade)==1){
    scanf(" %lf", &credits);
    switch(grade){
    case 'A':
    case 'a':
      points = 4.0;
      break;
    case 'B':
    case 'b':
      points = 3.0;
      break;
    case 'C':
    case 'c':
      points = 2.0;
      break;
    case 'D':
    case 'd':
      points = 1.0;
      break;
    }
    point_sum += (points*credits);
    credit_sum += credits;
    count++;
    printf("course %i: ", count);
  }

  if (credit_sum >0) {
    gpa = point_sum / credit_sum;
    printf("\nYour GPA is: %.2f\n", gpa);
    if (gpa >= 3.5){
      printf("Dean's List\n");
    } else if (gpa <= 2.0) {
      printf("Uh-oh, Academic Probation...\n");
    }
  } else {
        printf("No credits attempted; No GPA to report\n");
  }
    return 0;
}
