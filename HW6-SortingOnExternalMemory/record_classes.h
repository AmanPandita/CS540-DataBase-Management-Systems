#include <string>
#include <fstream>
#include <sstream>

class Records {
public:
    struct EmpRecord {
        int eid;
        std::string ename;
        int age;
        double salary;
    };

    EmpRecord emp_record;
    bool has_values = true;

    Records() {}

    Records Grab_Emp_Record(std::fstream &empin) {
        std::string line, word;

        if (getline(empin, line, '\n')) {
            std::stringstream s(line);
            getline(s, word, ',');
            emp_record.eid = std::stoi(word);
            getline(s, word, ',');
            emp_record.ename = word;
            getline(s, word, ',');
            emp_record.age = std::stoi(word);
            getline(s, word, ',');
            emp_record.salary = std::stod(word);

            return *this;
        } else {
            has_values = false;
            return *this;
        }
    }
};
