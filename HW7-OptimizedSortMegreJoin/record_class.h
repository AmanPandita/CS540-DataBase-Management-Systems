/* 
This is a skeleton code for Optimized Merge Sort, you can make modifications as long as you meet all question requirements
*/

/* 
This record_class.h contains the class Records, which can be used to store tuples form Emp.csv (stored in the EmpRecords) and Dept.csv (Stored in DeptRecords) files.
*/

#ifndef _CLASS_H_
#define _CLASS_H_

#include <iostream>
#include <string>

using namespace std;

// Dept (did (integer), dname (string), budget (double), managerid (integer))
// Fields of types integer,double, and string occupy 4, 8, and 40 bytes, respectively.

class Dept
{
    private:
        int did;
        string dname;
        double budget;
        int manager_id;

    public:

        Dept(int Id, string Name, double Budget, int Manager_id)
        {
            did = Id;
            dname = Name;
            budget = Budget;
            manager_id = Manager_id;
            
        }

        Dept()
        {
        }

        int get_did() { return did; }
        string get_dname() { return dname; }
        double get_budget() { return budget; }
        int get_manager_id() const { return manager_id; }

};

// Emp (eid (integer), ename (string), age (integer), salary (double))
class Emp
{
    private:
        int eid;
        string ename;
        int age;
        double salary;

    public:

        Emp(int Id, string Name, int Age, double Salary)
        {
            eid = Id;
            ename = Name;
            age = Age;
            salary = Salary;
        }

        Emp()
        {
        }

        int get_eid() const { return eid; }
        string get_ename() { return ename; }
        int get_age() { return age; }
        double get_salary() { return salary; }
};

#endif