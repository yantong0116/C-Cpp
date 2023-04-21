// Fig. 2.5: fig02_05.cpp
// Addition program that displays the sum of two integers.
#include <iostream> // enables program to perform input and output

// function main begins program execution
int main()
{
    // declaring and initializing variables
    int number1{0}; // first integer to add (initialized to 0)
    int number2{0}; // second integer to add (initialized to 0)
    int sum{0};     // sum of number1 and number2 (initialized to 0)

    std::cout << "Enter first integer: "; // prompt user for data
    std::cin >> number1;                  // read first integer from user into number1

    std::cout << "Enter second integer: "; // prompt user for data
    std::cin >> number2;                   // read second integer from user into number2

    sum = number1 + number2;                    // add the numbers; store result in sum
    std::cout << "Sum is " << sum << std::endl; // display sum; end line
} // end function main