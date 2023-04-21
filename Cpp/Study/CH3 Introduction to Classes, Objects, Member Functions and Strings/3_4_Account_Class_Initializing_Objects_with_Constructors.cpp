// Fig. 3.5: AccountTest.cpp
// Using the Account constructor to initialize the name data
// member at the time each Account object is created.
#include <iostream>
#include "3_4_Account.h"

using namespace std;

int main()
{
    // create two Account objects

    Account account1{"Jane Green"};
    Account account2{"John Blue"};

    // display initial value of name for each Account
    cout << "account1 name is: " << account1.getName() << endl;
    cout << "account2 name is: " << account2.getName() << endl;
}