// Fig. 4.10: ClassAverage.cpp
// Solving the class-average problem using counter-controlled iteration.
#include <iostream>
using namespace std;

int main()
{
    // initialization phase
    int total{0};                 // initialize sum of grades entered by the user
    unsigned int gradeCounter{1}; // initialize grade # to be entered next

    // processing phase uses counter-controlled iteration
    while (gradeCounter <= 10)
    {                            // loop 10 times
        cout << "Enter grade: "; // prompt
        int grade;
        cin >> grade;                    // input next grade
        total = total + grade;           // add grade to total
        gradeCounter = gradeCounter + 1; // increment counter by 1
    }

    // termination phase
    int average{total / 10}; // int division yields int result

    // display total and average of grades
    cout << "\nTotal of all 10 grades is " << total;
    cout << "\nClass average is " << average << endl;
}
