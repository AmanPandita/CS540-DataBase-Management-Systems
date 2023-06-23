// Submitted By: 
//Aman Pandita, panditaa@oregonstate.edu
//Manpreet Kaur, kaurmanp@oregonstate.edu


#include <string>
#include <ios>
#include <fstream>
#include <vector>
#include <string.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include "classes.h"
using namespace std;

int main(int argc, char *const argv[])
{

    BPlusTree tree;

    string csvFileName = "Employee.csv";
    string indexFileName = "EmployeeIndex.bin";
    string dataFileName = "DataFile.bin";

    fstream csvFile(csvFileName);
    fstream employeeIndex(indexFileName, ios::binary | ios::in | ios::out | ios::trunc);
    fstream dataFile(dataFileName, ios::binary | ios::in | ios::out | ios::trunc);

    // fstream dataFile(dataFileName, ios::in | ios::binary);

    string line;

    // getline(dataFile, line); // skip header

    long long dataPtr = dataFile.tellg();

    cout<< "Initiated\n";

    while (getline(csvFile, line))
    {
        cout<< "\nInserting from csv file\n";
        stringstream ss(line);

        string token;

        getline(ss, token, ',');

        int id = stoi(token);
        cout<<line<<"\n";
        tree.writeDataToFile(dataFileName, line);

        tree.insert(id, dataPtr);

        dataPtr = dataFile.tellg();
    }

    dataFile.close();
    
    tree.writeIndexToFile(indexFileName);
    cout<<"after write node to file in main\n";
    BPlusNode* root0 = tree.readIndexFromFile(indexFileName);

    cout<<"root: " <<root0<<"\n";

    cout << "Enter an ID to search: ";
    string input;
    cin >> input;
    int id = stoi(input);
    try
    {
        vector<long long> resultDataPtrs = tree.search(root0, id);

        dataFile.open(dataFileName, ios::in | ios::binary);
        // employeeIndex.open(indexFileName, ios::in | ios::binary);

        for (long long dataPtr : resultDataPtrs)
        {
            dataFile.seekg(dataPtr);
            // employeeIndex.seekg(dataPtr);
            // cout<<"in for loop\n";
            getline(dataFile, line);

            cout << line << endl;
        }
        cout<<"complete execution\n";

        dataFile.close();
    }
    catch (const std::runtime_error &e)
    {
        cout << e.what() << '\n';
    }

    // int searchId = stoi(argv[1]);

    return 0;
}

// #include <fstream>
// #include <vector>
// #include <string>
// #include <iostream>
// #include <sstream>
// #include <stdexcept>
// #include <cmath>
// #include "classes.h"
// using namespace std;

// int main(int argc, char* const argv[]) {
//     // Create the B+ tree
//     BPlusTree tree;

//     // Create the EmployeeIndex file and DataFile
//     fstream employeeIndex("EmployeeIndex.bin", ios::binary | ios::in | ios::out | ios::trunc);
//     fstream dataFile("DataFile.bin", ios::binary | ios::in | ios::out | ios::trunc);

//     // Insert records from Employee.csv to the DataFile and relevant information in the EmployeeIndex file
//     tree.createFromFile("Employee.csv");

//     // // Call this function after the tree is created
//     // tree.printBPlusTree(tree.root);

//     // Loop to lookup IDs until user is ready to quit
//     while (true) {
//         cout << "Enter an ID to search for, or 'q' to quit: ";
//         string input;
//         cin >> input;
//         if (input == "q") {
//             break;
//         }
//         int id = stoi(input);
//         try {
//             Record record = tree.findRecordById(id);
//             record.print();
//         } catch (const std::runtime_error& e) {
//             cout << e.what() << '\n';
//         }
//     }

//     return 0;
// }