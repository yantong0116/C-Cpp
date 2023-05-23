// Fig. 5.1: WhileCounter.cpp
// Counter-controlled iteration with the while iteration statement.

#include <iostream>
using namespace std;

int main(){
    unsigned int counter{1};

    while(counter <= 10){
        cout << counter << "  ";
        ++counter;
    }

    cout << endl;
}