// Fig. 3.6: fig03_06.c
// Class average program with counter-controlled iteration.
#include <stdio.h>
// function main begins program execution
int main(void)
{
    unsigned int counter;
    int grade;   // grade value
    int total;   // sum of grades entered by user
    int average; // average of grades

    // initialization phase
    total = 0; // initialize total
    counter = 1;

    // processing phase
    while (counter <= 10)
    {                                  // loop 10 times
        printf("%s", "Enter grade: "); // prompt for input
        scanf("%d", &grade);           // read grade from user
        total = total + grade;         // add grade to total
        counter = counter + 1;
    } // end while

    // termination phase
    average = total / 10; // integer division

    printf("Class average is %d\n", average); // display result
} // end function main