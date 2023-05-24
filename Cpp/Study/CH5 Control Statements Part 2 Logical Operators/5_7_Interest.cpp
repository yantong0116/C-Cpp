// Fig. 5.7: Interest.cpp
// Compound-interest calculations with class DollarAmount and integers.
#include <iostream>
#include <iomanip>
#include <string>
#include "5_7_DollarAmount.h"
using namespace std;

int main()
{
    DollarAmount d1{12345}; // $123.45
    DollarAmount d2{1576};  // $15.76

    cout << "After adding d2 (" << d2.toString() << ") into d1 (" << d1.toString() << "), d1 = ";
    d1.add(d2); // modifies object d1
    cout << d1.toString() << "\n";

    cout << "After subtracting d2 (" << d2.toString() << ") from d1 (" << d1.toString() << "), d1 = ";
    d1.subtract(d2); // modifies object d1
    cout << d1.toString() << "\n";

    cout << "After subtracting d1 (" << d1.toString() << ") from d2 (" << d2.toString() << "), d2 = ";
    d2.subtract(d1); // modifies object d2
    cout << d2.toString() << "\n\n";

    cout << "Enter integer interest rate and divisor. For example:\n"
         << "for 2%, enter: 2 100\n"
         << "for 2.3%, enter: 23 1000\n"
         << "for 2.37%, enter: 237 10000\n"
         << "for 2.375%, enter: 2375 100000\n> ";
    int rate;    // whole-number interest rate
    int divisor; // divisor for rate
    cin >> rate >> divisor;

    DollarAmount balance{100000}; // initial principal amount in pennies
    cout << "\nInitial balance: " << balance.toString() << endl;

    // display headers
    cout << "\nYear" << setw(20) << "Amount on deposit" << endl;

    // calculate amount on deposit for each of ten years
    for (unsigned int year{1}; year <= 10; year++)
    {
        // increase balance by rate % (i.e., rate / divisor)
        balance.addInterest(rate, divisor);

        // display the year and the amount
        cout << setw(4) << year << setw(20) << balance.toString() << endl;
    }
}