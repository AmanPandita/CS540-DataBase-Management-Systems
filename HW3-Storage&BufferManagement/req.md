# What you must do



Assume that we have a relation Employee(id, name, bio, manager-id). The values of id and manager-id are integers each with the fixed size of 8 bytes. The values of name and bio are character strings and take at most 200 and 500 bytes, respectively. Note that as opposed to the values of id and manager-id, the sizes of the values of name and bio are not fixed and are between 1 to 200 (500) bytes. The size of each page is 4096 bytes (4KB). The size of each record is less than the size of a page. Using the provided skeleton code with this assignment, write a C++ program that stores relation Employee and access its records in a single file on the external storage, i.e., hard disk. 

Your program must first read an input Employee relation and store in a new file on disk. The input relation is stored in a CSV file, i.e., each tuple is in a separate line and fields of each record are separated by commas. Your program must assume that the input CSV file is in the current working directory, i.e., the one from which your program is running, and its name is Employee.csv. We have included an input CSV file with this assignment as a sample test case for your program. Your program must create the new file and store and access records on the new file  correctly for other CSV files with the same fields as the sample file.

Your program must store the records of the input CSV file in a new file with the name EmployeeRelation on the current working directory. You may use one of the methods explained for storing variable-length records and the method described on storing pages (blocks) of variable-length records in our lectures on storage management to store records and pages. They are also explained in Sections 9.7.2 and 9.6.2 of Cow Book, respectively. 

During the file creation, your program may keep up to three pages (blocks) in main memory at any time. The submitted solutions that use more main memory will not get any points.

After finishing the file creation, your program should accept an Employee id in its command line and search the file for all records of the given id. Like file creation, your program may use up to three pages in main memory at any time. The submitted solutions that use more main memory will not get any points for implementing this lookup operation. The user of your program may search for records of multiple ids, one id at a time.

Each student has an account on hadoop-master.engr.oregonstate.edu server, which is a Linux machine. Your should ensure that your program can be compiled and run on this machine. You can use the following bash command to connect to it:

> ssh your_onid_username@hadoop-master.engr.oregonstate.edu

Then it asks for your ONID password and probably one another question. To access this server, you must be on campus or connect to the Oregon State VPN.

You can use following commands to compile and run C++ code:
> g++ -std=c++11 main.cpp -o main.out

> main.out
