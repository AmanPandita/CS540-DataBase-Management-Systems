#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

#include "record_class.h"

using namespace std;

#define MAIN_MEMORY_SIZE 22

vector<Dept> main_memory1;
vector<Emp> main_memory2;

// To compare manager_id value
bool compare_id(const Dept &n1, const Dept &n2)
{
	return n1.get_manager_id() < n2.get_manager_id();
}

// To compare eid value
bool compare_eid(const Emp &n1, const Emp &n2)
{
	return n1.get_eid() < n2.get_eid();
}

/*
read_csv function to Read csv file "Dept.csv" and make Dept object using csv data Save to vector<Dept> dept_list

*/

vector<Dept> read_csv(vector<Dept> elist, string file_name)
{
	// Read the CSV file and store the data in a vector of Department objects.
	std::ifstream infile(file_name);
	std::string line;
	std::vector<std::string> tokens;
	while (std::getline(infile, line))
	{
		tokens.clear();
		std::stringstream ss(line);
		std::string token;
		while (std::getline(ss, token, ','))
		{
			tokens.push_back(token);
		}

		int did = std::stoi(tokens[0]);
		std::string dname = tokens[1];
		double budget = std::stod(tokens[2]);
		int manager_id = std::stoi(tokens[3]);

		Dept data1(did, dname, budget, manager_id);
		elist.push_back(data1);
	}

	return elist;
}

/*
Dept_temp_sort function: By using dept_list, make temp sorted csv file, named "Dept_temp.csv" Sorted by memory size

*/

int Dept_temp_sort(vector<Dept> dept_list, vector<Dept> main_memory1, string filename)
{
	// This function sorts the department list and saves it to a temporary CSV file.

	// Initialize the number of blocks.
	int block_n = (dept_list.size() / 22) + 1;

	// Create an empty department object.
	Dept empty;

	// Create a vector iterator for the department list.
	vector<Dept>::iterator iter2 = dept_list.begin();

	// Create an output file stream.
	ofstream fs(filename);

	// Set the precision for the output file stream.
	fs.precision(10);

	// Loop until the department list is empty.
	while (dept_list.size() != 0)
	{
		// If the file is open.
		if (fs.is_open())
		{
			// If the number of departments in the list is greater than the main memory size.
			if (dept_list.size() > MAIN_MEMORY_SIZE)
			{
				// Clear the main memory.
				main_memory1.clear();

				// Assign the first `MAIN_MEMORY_SIZE` departments to the main memory.
				main_memory1.assign(dept_list.begin(), dept_list.begin() + MAIN_MEMORY_SIZE);

				// Sort the main memory.
				sort(main_memory1.begin(), main_memory1.end(), compare_id);

				// Erase the first `MAIN_MEMORY_SIZE` departments from the department list.
				dept_list.erase(iter2, iter2 + MAIN_MEMORY_SIZE);

				// Create an iterator for the end of the main memory.
				vector<Dept>::iterator iterEnd = main_memory1.end();

				// Loop over the main memory and save the departments to the file.
				for (vector<Dept>::iterator iter_p = main_memory1.begin(); iter_p != iterEnd; ++iter_p)
				{
					fs << iter_p->get_did() << "," << iter_p->get_dname() << "," << iter_p->get_budget() << "," << iter_p->get_manager_id() << "\n";
				}
			}
			else
			{
				// Clear the main memory.
				main_memory1.clear();

				// Assign the remaining departments to the main memory.
				main_memory1.assign(dept_list.begin(), dept_list.end());

				// Sort the main memory.
				sort(main_memory1.begin(), main_memory1.end(), compare_id);

				// Erase all of the departments from the department list.
				dept_list.clear();

				// Create an iterator for the end of the main memory.
				vector<Dept>::iterator iterEnd = main_memory1.end();

				// Loop over the main memory and save the departments to the file.
				for (vector<Dept>::iterator iter_p = main_memory1.begin(); iter_p != iterEnd; ++iter_p)
				{
					fs << iter_p->get_did() << "," << iter_p->get_dname() << "," << iter_p->get_budget() << "," << iter_p->get_manager_id() << "\n";
				}
			}
		}
	}

	// Close the file stream.
	fs.close();

	// Return the number of blocks.
	return block_n;
}

/*
Dept_sort function to Read "Dept_temp.csv" and save to 2 dimention array named arr, Sort all of data and save to dept_list
*/

vector<Dept> Dept_sort(int block_n, vector<Dept> dept_list, vector<Dept> main_memory1, string filename)
{

	// To use save value
	int did;
	string dname;
	int manager_id;
	double budget;

	// Used variables
	int current_num = 0;
	int count = 0;
	int comp;
	int index;

	// Define 2 Dimension array
	Dept empty;
	vector<vector<Dept>> arr(block_n, vector<Dept>(MAIN_MEMORY_SIZE, empty));

	// file open
	string str_buf;
	fstream fs;
	fs.open(filename, ios::in);

	// Clear array
	arr[current_num].clear();

	while (1)
	{

		getline(fs, str_buf, ',');
		if (str_buf.empty())
		{
			break;
		}
		did = stoi(str_buf);

		getline(fs, str_buf, ',');
		dname = str_buf;

		getline(fs, str_buf, ',');
		budget = stod(str_buf);

		getline(fs, str_buf, '\n');
		manager_id = stoi(str_buf);

		Dept *data1 = new Dept(did, dname, budget, manager_id);

		if (count < MAIN_MEMORY_SIZE)
		{
			arr[current_num].push_back(*data1);
			count++;
		}
		else
		{
			current_num += 1;
			arr[current_num].clear();
			arr[current_num].push_back(*data1);
			count = 1;
		}
		delete (data1);
	}
	fs.close();

	if (block_n > (MAIN_MEMORY_SIZE - 1))
	{
		throw out_of_range("Exceed maximum block number");
	}
	else
	{
		main_memory1.clear();

		for (int j = 0; j < block_n; j++)
		{
			main_memory1.push_back(arr[j][0]);
			arr[j].erase(arr[j].begin());
		}

		comp = main_memory1[0].get_manager_id();
		while (!main_memory1.empty())
		{
			index = 0;
			vector<Dept>::iterator iterEnd = main_memory1.end();
			for (vector<Dept>::iterator iter_p = main_memory1.begin(); iter_p != iterEnd; ++iter_p)
			{
				if (iter_p->get_manager_id() < comp)
				{
					index = distance(main_memory1.begin(), iter_p);
				}
			}

			dept_list.push_back(main_memory1[index]);

			if (arr[index].empty())
			{
				main_memory1.erase(main_memory1.begin() + index);
			}
			else
			{
				if (index == 0)
				{
					comp = arr[index][0].get_manager_id();
				}
				main_memory1[index] = arr[index][0];
				arr[index].erase(arr[index].begin());
			}
		}
	}
	return dept_list;
}


/*
read_csv function to Read csv file "Emp.csv" and make Emp object using csv data & Save to vector<Emp> emp_list
*/
vector<Emp> read_csv(vector<Emp> employeeList, const string &filename)
{
	// Variables used in the CSV reading process
	string line;
	fstream fileStream;

	int employeeId;
	string employeeName;
	int employeeAge;
	double employeeSalary;

	// Open the file
	fileStream.open(filename, ios::in);

	// Read the file line by line
	while (getline(fileStream, line, ','))
	{
		// Parse each column of the CSV line
		employeeId = stoi(line);

		getline(fileStream, line, ',');
		employeeName = line;

		getline(fileStream, line, ',');
		employeeAge = stoi(line);

		getline(fileStream, line, '\n');
		employeeSalary = stod(line);

		// Create an Emp object and add it to the list
		Emp employeeData(employeeId, employeeName, employeeAge, employeeSalary);
		employeeList.push_back(employeeData);
	}

	// Close the file
	fileStream.close();

	return employeeList;
}

/*
Emp_temp_sort function to By using emp_list, make temp sorted csv file, named "Emp_temp.csv" & Sorted by memory size
*/

int Emp_temp_sort(std::vector<Emp> employeeList, std::vector<Emp> mainMemory, std::string filename)
{
	const int blockSize = 22;
	int numberOfBlocks = (employeeList.size() / blockSize) + 1;

	std::ofstream fileStream(filename);
	fileStream.precision(10);

	while (!employeeList.empty())
	{
		if (fileStream.is_open())
		{
			int dataAmount = (employeeList.size() > MAIN_MEMORY_SIZE) ? MAIN_MEMORY_SIZE : employeeList.size();

			mainMemory.clear();
			mainMemory.assign(employeeList.begin(), employeeList.begin() + dataAmount);
			std::sort(mainMemory.begin(), mainMemory.end(), compare_eid);

			employeeList.erase(employeeList.begin(), employeeList.begin() + dataAmount);

			for (std::vector<Emp>::iterator it = mainMemory.begin(); it != mainMemory.end(); ++it)
			{
				fileStream << it->get_eid() << "," << it->get_ename() << "," << it->get_age() << "," << it->get_salary() << "\n";
			}
		}
	}
	fileStream.close();
	return numberOfBlocks;
}

/*
name: Emp_sort_join( function:   Read "Emp_temp.csv" and save to 2 dimention array named arr Sort all of data and join with dept_list at the same time Save joined table to Join.csv file
*/


void Emp_sort_join(int block_n2, vector<Dept> dept_list, vector<Emp> emp_list, vector<Emp> main_memory2, string filename, fstream &fout)
{
	// Temporarily storing values
	int eid;
	string ename;
	int age;
	double salary;

	// Variable initialization
	int count = 0, current_num = 0, comp, index, dept_idx = 0, add = 0;
	bool flag = true;

	// Define a 2D array
	Emp empty;
	vector<Emp>::iterator iter2 = emp_list.begin();
	vector<vector<Emp>> arr(block_n2, vector<Emp>(MAIN_MEMORY_SIZE, empty));

	// File opening
	string str_buf;
	fstream fs;
	fs.open(filename, ios::in);

	// Increasing file writing precision
	fout.precision(10);

	// Clearing array
	arr[current_num].clear();

	// Infinite loop to read from file
	do
	{
		getline(fs, str_buf, ',');
		if (str_buf.empty())
			break;
		eid = stoi(str_buf);

		getline(fs, str_buf, ',');
		ename = str_buf;

		getline(fs, str_buf, ',');
		age = stoi(str_buf);

		getline(fs, str_buf, '\n');
		salary = stod(str_buf);

		// Temporary Dept object data creation
		Emp *data1 = new Emp(eid, ename, age, salary); 

		// If arr[current_num] can contain maximum MAIN_MEMORY_SIZE values
		if (count < MAIN_MEMORY_SIZE)
		{
			arr[current_num].push_back(*data1); // Save to array
			count++;
		}
		else
		{
			++current_num;
			arr[current_num].clear();				 
			arr[current_num].push_back(*data1); 
			count = 1;
		}
	} while (true);

	fs.close(); // Closing the file

	if (block_n2 > (MAIN_MEMORY_SIZE - 1))
		throw out_of_range("Exceed maximum block number"); // Error handling

	else
	{
		main_memory2.clear(); 

		for (int j = 0; j < block_n2; j++) 
		{
			main_memory2.push_back(arr[j][0]);
			arr[j].erase(arr[j].begin());	
		}

		comp = main_memory2[0].get_eid();

		while (!main_memory2.empty())	
		{
			index = 0;
			vector<Emp>::iterator iterEnd = main_memory2.end();
			for (vector<Emp>::iterator iter_p = main_memory2.begin(); iter_p != iterEnd; ++iter_p)
			{
				if (iter_p->get_eid() < comp)
				{
					index = distance(main_memory2.begin(), iter_p);
				}
			}

			emp_list.push_back(main_memory2[index]); 

			add = 0;
			flag = true;
			while (flag)
			{
				if (dept_idx >= dept_list.size() - 1) 
					flag = false;

				if (emp_list.back().get_eid() > dept_list[dept_idx].get_manager_id())
					dept_idx++;

				if (emp_list.back().get_eid() < dept_list[dept_idx].get_manager_id())
					flag = false; 

				if (emp_list.back().get_eid() == dept_list[dept_idx].get_manager_id())
				{
					// Creating a join tuple
					fout << dept_list[dept_idx].get_did() << ',' << dept_list[dept_idx].get_dname() << ',' << dept_list[dept_idx].get_budget()
						 << ',' << dept_list[dept_idx].get_manager_id() << ',' << emp_list.back().get_eid() << ','
						 << emp_list.back().get_ename() << ',' << emp_list.back().get_age() << ',' << emp_list.back().get_salary() << "\n";

					while (1) // Checking for duplicates
					{
						if (dept_list[dept_idx + add].get_manager_id() == dept_list[dept_idx + add + 1].get_manager_id())
						{
							// Creating a join tuple
							fout << dept_list[dept_idx + add + 1].get_did() << ',' << dept_list[dept_idx + add + 1].get_dname() << ',' << dept_list[dept_idx + add + 1].get_budget()
								 << ',' << dept_list[dept_idx + add + 1].get_manager_id() << ',' << emp_list.back().get_eid() << ','
								 << emp_list.back().get_ename() << ',' << emp_list.back().get_age() << ',' << emp_list.back().get_salary() << "\n";

							add++;
						}
						else
							break;
					}
					flag = false; // To end infinite loop
				}
			}

			if (arr[index].empty())
				main_memory2.erase(main_memory2.begin() + index); // Delete from main memory to avoid comparison

			else
			{
				if (index == 0)
					comp = arr[index][0].get_eid(); // Resetting compare value
				
				main_memory2[index] = arr[index][0];  
				arr[index].erase(arr[index].begin()); 
			}
		}
	}
}


int main(int argc, const char *argv[])
{
	// Define dept_list
	std::vector<Dept> dept_list;

	// Define emp_list
	std::vector<Emp> emp_list;

	// File stream
	std::fstream joinout;
	joinout.open("Join.csv", std::ios::out | std::ios::app);

	// Used variables
	int block_n, block_n2;

	// Read csv files and save to vector list
	dept_list = read_csv(dept_list, "Dept.csv");
	emp_list = read_csv(emp_list, "Emp.csv");

	// Temporary sort and save dept_list to "Dept_temp.csv"
	block_n = Dept_temp_sort(dept_list, main_memory1, "Dept_temp.csv");
	dept_list.clear();

	// Finally sort dept_list
	dept_list = Dept_sort(block_n, dept_list, main_memory1, "Dept_temp.csv");

	// Temporary sort and save emp_list to "Emp_temp.csv"
	block_n2 = Emp_temp_sort(emp_list, main_memory2, "Emp_temp.csv");
	emp_list.clear();

	// Sort emp_list and join with dept during the merge phase
	Emp_sort_join(block_n2, dept_list, emp_list, main_memory2, "Emp_temp.csv", joinout);

	// Delete temporary files
	std::remove("Dept_temp.csv");
	std::remove("Emp_temp.csv");

	return 0;
}