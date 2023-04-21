// Fig. 3.1: AccountTest.cpp
// Creating and manipulating an Account object.
#include <iostream>
#include <string>
#include "3_2_Account.h"

using namespace std;

int main()
{
    Account myAccount; // create Account object myAccount

    // show that the initial value of myAccount's name is the empty string
    cout << "Initial account name is: " << myAccount.getName();

    // prompt for and read name
    cout << "\nPlease enter the account name: ";
    string theName;

    getline(cin, theName);      // read a line of text
    myAccount.setName(theName); // put theName in myAccount

    // display the name stored in object myAccount
    cout << "Name in object myAccount is: " << myAccount.getName() << endl;
}