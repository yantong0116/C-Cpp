#include <stdio.h>

int square(int y);  // function prototype

int main(void){
    // loop 10 times and calculate and output square of x each time
    for(int x = 1; x <= 10; ++x){
        printf("%d ", square(x));
    }
    
    puts("");
}

int square(int y){
    return y * y;
}