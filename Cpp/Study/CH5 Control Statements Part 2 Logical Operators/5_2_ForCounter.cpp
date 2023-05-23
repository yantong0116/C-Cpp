// Fig. 5.2: ForCounter.cpp
// Counter-controlled iteration with the for iteration statement.

#include <iostream>
using namespace std;

int main(){
    for(unsigned int counter{1}; counter <= 10; ++counter){
        cout << counter << "  ";
    }
    cout << endl;
}