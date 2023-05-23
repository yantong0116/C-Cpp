// Fig. 5.5: Sum.cpp
// Summing integers with the for statement.

#include <iostream>
using namespace std;

int main(){
    unsigned int total{0};

    // total even integers from 2 through 20
    for(unsigned int number{2}; number <= 20; number += 2){
        total += number;
    }
    cout << "Sum is " << total << endl;
}

