#include <bits/stdc++.h>
#include "record_classes.h"

#define buffer_size 22

Records buffers[buffer_size];

void sortBuffer()
{
    std::sort(buffers, buffers + buffer_size, [](const Records &recordA, const Records &recordB)
              { return recordA.emp_record.eid < recordB.emp_record.eid; });
}

struct CompareRecords
{
    bool operator()(const std::pair<Records, int> &recordPairA, const std::pair<Records, int> &recordPairB)
    {
        return recordPairA.first.emp_record.eid > recordPairB.first.emp_record.eid;
    }
};

void mergeRuns()
{
    // Prefix and extension for the temporary chunk files
    std::string tempFilePrefix = "chunk";
    std::string tempFileExtension = ".txt";

    // Priority queue to hold pairs of Records and their file indices, sorted by Records
    std::priority_queue<std::pair<Records, int>, std::vector<std::pair<Records, int>>, CompareRecords> minHeap;

    // Vector to hold the file streams for each temporary file
    std::vector<std::fstream> tempFiles(buffer_size);

    // Open each temporary file and push the first record into the min heap
    for (int i = 0; i < buffer_size; ++i)
    {
        tempFiles[i].open(tempFilePrefix + std::to_string(i) + tempFileExtension, std::ios::in);
        if (tempFiles[i].is_open())
        {
            Records record;
            record = record.Grab_Emp_Record(tempFiles[i]);
            if (record.has_values)
            {
                minHeap.push({record, i});
            }
        }
    }

    // Open the output file
    std::fstream sortedOutput;
    sortedOutput.open("EmpSorted.csv", std::ios::out | std::ios::app);

    // Continuously remove the smallest record and add the next record from the same file
    while (!minHeap.empty())
    {
        std::pair<Records, int> smallestRecord = minHeap.top();
        minHeap.pop();

        Records &record = smallestRecord.first;
        int fileIndex = smallestRecord.second;

        // Write the smallest record to the output file
        sortedOutput << record.emp_record.eid << "," << record.emp_record.ename << ","
                     << record.emp_record.age << "," << record.emp_record.salary << "\n";

        // Fetch the next record from the same file and push it into the min heap
        Records nextRecord;
        nextRecord = nextRecord.Grab_Emp_Record(tempFiles[fileIndex]);
        if (nextRecord.has_values)
        {
            minHeap.push({nextRecord, fileIndex});
        }
    }

    // Close and delete all temporary files
    for (int i = 0; i < buffer_size; ++i)
    {
        tempFiles[i].close();
        std::remove((tempFilePrefix + std::to_string(i) + tempFileExtension).c_str());
    }

    // Close the output file
    sortedOutput.close();
}

void writeSortedRecords(std::fstream &outputStream)
{
    for (int i = 0; i < buffer_size; ++i)
    {
        if (buffers[i].has_values) {
            const Records::EmpRecord &employee = buffers[i].emp_record;
            outputStream << employee.eid << "," << employee.ename << ","
                         << employee.age << "," << employee.salary << "\n";
        }
    }
}

void processAndSortChunks(std::fstream &inputFile)
{
    int runCount = 0;
    std::string tempFilePrefix = "chunk";
    std::string tempFileExtension = ".txt";

    // Loop until EOF is reached
    while (inputFile.peek() != EOF)
    {
        // Open a new file for each chunk
        std::fstream chunkFile;
        chunkFile.open(tempFilePrefix + std::to_string(runCount) + tempFileExtension, std::ios::out | std::ios::app);

        // Read buffer_size records
        for (int i = 0; i < buffer_size; ++i)
        {
            buffers[i] = buffers[i].Grab_Emp_Record(inputFile);
        }

        // Sort the records in memory
        sortBuffer();

        // Write sorted records to the chunk file
        writeSortedRecords(chunkFile);

        // Close the chunk file and increment the run count for the next file
        chunkFile.close();
        runCount++;
    }

    // Merge all the sorted chunk files into one sorted file
    mergeRuns();
}

int main()
{
    // Open the CSV file for reading
    std::fstream employeeDataFile;
    employeeDataFile.open("Emp.csv", std::ios::in);

    // Process and sort the chunks of data
    processAndSortChunks(employeeDataFile);

    // Close the file after processing
    employeeDataFile.close();

    return 0;
}
