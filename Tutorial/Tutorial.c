#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct _s1 {
    char a[5];
} s1;

int main() {
    s1 p[10];
    printf("struct s1 size: %ld byte\n", sizeof(s1));
    for(int i = 0; i < 10; i++) {
        printf("the struct p[%d] address =%p\n", i, p + i);
    }
}