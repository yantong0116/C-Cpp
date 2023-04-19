// Fig. 2.5: fig02_05.c
// Addition program.
#include <stdio.h>

// function main begins program execution
int main(void)
{
    int integer1;
    int integer2;

    printf("Enter first integer : \n"); // prompt
    scanf("%d", &integer1);

    printf("Enter second integer : \n"); // prompt
    scanf("%d", &integer2);

    int sum;
    sum = integer1 + integer2; // assign total to sum

    printf("Sum is %d\n", sum);
} // end function main