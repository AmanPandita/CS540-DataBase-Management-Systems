/*




Submitted By:
Aman Pandita, panditaa@oregonstate.edu
Manpreet Kaur, kaurmanp@oregonstate.edu



Skeleton code for linear hash indexing
*/

#include <ios>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include "classes.h"
using namespace std;


int main(int argc, char* const argv[]) {

    // Create the index
    LinearHashIndex emp_index("EmployeeIndex.bin");
    emp_index.createFromFile("Employee.csv");
    
    int user_input;
    // Loop to lookup IDs or negative value or 0 to quit
    while (true)
    {
        
		cout << "Enter ID\n";
		cout << "Enter 0 or negative number to quit\n";
		
        cout << ">> ";
        cin >> user_input;

        // Validate user input
        if(cin.fail()) {

            do {
                cout << "Error: Invalid ID!\n";
                cin.clear();
                cin.ignore(256, '\n');
                cout << ">> ";
                cin >> user_input;
            } while (cin.fail());
        }

        // Search or quit
        if (user_input > 0) {

            cout << "Searching for ID " << user_input << "...\n";

            Record targetRecord = emp_index.findRecordById(user_input);

            // Print record
            targetRecord.print();

        }
        else
            return 0;

    } 
    
    return 0;
}
