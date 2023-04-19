#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>

typedef struct _s1 {
    char a[5];
} s1;

int x = 5;
int y = 2;
float sum = (float) 5 / 2;
const int BIRTHYEAR = 1999;
bool isProgrammingFun = true;
bool isFishTasty = false;
int myNum = 10;
int time = 20;
int day = 4;
int loopi = 0;
int myNumbers[] = {25, 50, 75, 100};
char greetings[] = "Hello World!";
int InputNumber;
char Name[20];
char fullName[30];

int main() {
    s1 p[10];
    printf("struct s1 size: %ld byte\n", sizeof(s1));
    for(int i = 0; i < 10; i++) {
        printf("the struct p[%d] address =%p\n", i, p + i);
    }
    printf("%.1f\n", sum);
    printf("%d\n", BIRTHYEAR);
    printf("%d\n", isProgrammingFun);   // Returns 1 (true)
    printf("%d\n", isFishTasty);        // Returns 0 (false)

    if (myNum > 0) {
        printf("The value is a positive number.\n");
    } else if (myNum < 0) {
    printf("The value is a negative number.\n");
    } else {
    printf("The value is 0.\n");
    }

    (time < 18) ? printf("Good day.\n") : printf("Good evening.\n");

    switch (day) {
        case 1:
            printf("Monday");
            break;
        case 2:
            printf("Tuesday");
            break;
        case 3:
            printf("Wednesday");
            break;
        case 4:
            printf("Thursday");
            break;
        case 5:
            printf("Friday");
            break;
        case 6:
            printf("Saturday");
            break;
        case 7:
            printf("Sunday");
            break;
    }

    while (loopi < 5) {
        printf("%d\n", loopi);
        loopi++;
    }

    printf("%d\n", myNumbers[0]);

    greetings[0] = 'J';
    printf("%s\n", greetings);

    printf("Type a number: ");
    scanf("%d", &InputNumber);
    printf("Type your name: ");
    scanf("%s", &Name);
    // printf("Type your full name: \n");
    // fgets(fullName, sizeof(fullName), stdin);

    printf("Your input number is: %d\n", InputNumber);
    printf("Your full name is: %s\n", Name);
    // printf("Hello %s", fullName);

    printf("%p", &InputNumber);
}
