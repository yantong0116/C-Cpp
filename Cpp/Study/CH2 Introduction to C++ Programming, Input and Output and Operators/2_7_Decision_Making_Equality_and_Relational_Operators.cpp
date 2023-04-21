// Fig. 2.13: fig02_13.cpp
// Comparing integers using if statements, relational operators
// and equality operators.
#include <iostream> // enables program to perform input and output

using std::cin;
using std::cout;
using std::endl;

// function main begins program execution
int main()
{
    int number1{0}; // first integer to compare (initialized to 0)
    int number2{0}; // second integer to compare (initialized to 0)

    cout << "Enter two integers to compare: "; // prompt user for data
    cin >> number1 >> number2;

    if (number1 == number2)
    {
        cout << number1 << "==" << number2 << endl;
    }

    if (number1 != number2)
    {
        cout << number1 << " != " << number2 << endl;
    }

    if (number1 < number2)
    {
        cout << number1 << " < " << number2 << endl;
    }

    if (number1 > number2)
    {
        cout << number1 << " > " << number2 << endl;
    }

    if (number1 <= number2)
    {
        cout << number1 << " <= " << number2 << endl;
    }

    if (number1 >= number2)
    {
        cout << number1 << " >= " << number2 << endl;
    }
} // end function main