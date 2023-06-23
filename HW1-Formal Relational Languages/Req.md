# What You Must Do


## Consider the following relational schema:

Emp (eid: integer, ename: string, age: integer, salary: real)
Works (eid: integer, did: integer, pc_time: integer)
Dept (did: integer, dname: string, budge: real, managerid: integer)

The underlined attributes are keys for their relations. Note that a manager is an employee as well and their manager id and employee id are the same. An employee can work in more than one department. The pct_time field of the works relation shows the percentage of time that a given employee works in a given department and is always greater than zero. Write the following queries using Datalog, relational algebra, and relational calculus. 


    Return names of every employee who works in the "Hardware", "Software", and "Research" departments. (1.5 points)
    Return the names of every department without any employee. (1.5 points)
    Print the managerid of managers who manage only departments with budgets greater than $1.5 million. (1.5 points)
    Print the name of employees whose salary is less than or equal to the salary of every employee. (1.5 points)

