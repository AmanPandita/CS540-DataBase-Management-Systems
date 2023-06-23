#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <bitset>

using namespace std;

// const int d = 4; // degree of B+ tree
const int degree = 4;

// Class representing a tuple from the Employee.csv file
class Record
{
public:
    int id, manager_id;
    std::string bio, name;

    Record() {}

    Record(vector<std::string> fields)
    {
        id = stoi(fields[0]);
        name = fields[1];
        bio = fields[2];
        manager_id = stoi(fields[3]);
    }

    void print()
    {
        cout << "\tID: " << id << "\n";
        cout << "\tNAME: " << name << "\n";
        cout << "\tBIO: " << bio << "\n";
        cout << "\tMANAGER_ID: " << manager_id << "\n";
    }
};

// Class representing a node in the B+ tree
class BPlusNode
{
public:
    // vector<int> keys;             // Vector to store keys
    int keys[2 * degree];
    BPlusNode* children[2 * degree + 1]; // Vector to store child nodes
    bool is_leaf;                 // Boolean value indicating whether the node is a leaf node or not
    int size;                     // Size of the node
    long long dataPtrs[2 * degree];
    BPlusNode* next;
    // BPlusNode* parent; // Pointer to the parent node

    // Constructor to initialize the node
    BPlusNode(bool is_leaf = false) : is_leaf(is_leaf) {}

    // Use the Record structure to create necessary data structure to do the following:
    // The pointers in the leaf nodes of the index must keep the addresses of their
    // corresponding records on the created data file.
    Record record;
};

// Class representing the B+ tree
class BPlusTree
{
public:
    // Necessary variables
    BPlusNode *root; // Pointer to the root node
    // int degree;      // Degree of the B+ tree

    // Constructor to initialize the B+ tree
    BPlusTree() : root(nullptr) {}

    void splitChild(BPlusNode *current, int i)
    {
        cout<<"split child\n";
        BPlusNode *z = new BPlusNode();
        BPlusNode *y = current->children[i];
        z->is_leaf = y->is_leaf;
        z->size = degree - 1;
        for (int j = 0; j < degree - 1; j++)
        {
            z->keys[j] = y->keys[j + degree];
            z->dataPtrs[j] = y->dataPtrs[j + degree];
        }
        if (!y->is_leaf)
        {
            for (int j = 0; j < degree; j++)
            {
                z->children[j] = y->children[j + degree];
            }
        }
        y->size = degree - 1;
        for (int j = current->size; j >= i + 1; j--)
        {
            current->children[j + 1] = current->children[j];
        }
        current->children[i + 1] = z;
        for (int j = current->size - 1; j >= i; j--)
        {
            current->keys[j + 1] = current->keys[j];
            current->dataPtrs[j + 1] = current->dataPtrs[j];
        }
        current->keys[i] = y->keys[degree - 1];
        current->dataPtrs[i] = y->dataPtrs[degree - 1];
        current->size++;
    }

    void insertNonFull(BPlusNode *current, int k, long long dataPtr)
    {
        cout<<"insertnonfull\n";
        int i = current->size - 1;
        if (current->is_leaf)
        {
            cout<<"current node is leaf\n";
            while (i >= 0 && k < current->keys[i])
            {
                current->keys[i + 1] = current->keys[i];
                current->dataPtrs[i + 1] = current->dataPtrs[i];
                i--;
            }
            cout<<"after while\n";
            current->keys[i + 1] = k;
            current->dataPtrs[i + 1] = dataPtr;
            current->size++;
            cout<<"before if\n";
            if (i == current->size - 2)
            {
                cout<<"inside if\n";
                BPlusNode *nextNode = current->next;
                while (nextNode != nullptr)
                {
                    cout<<"inside while\n";
                    nextNode->keys[0] = k;
                    nextNode = nextNode->next;
                }
            }
        }
        else
        {
            cout<<"internal node\n";
            while (i >= 0 && k < current->keys[i])
            {
                i--;
            }
            i++;
            if (current->children[i]->size == 2 * degree - 1)
            {
                splitChild(current, i);
                if (k > current->keys[i])
                {
                    i++;
                }
            }
            insertNonFull(current->children[i], k, dataPtr);
        }
    }

    // Method to insert a key (id) in the B+ tree (to the index_file)
    void insert(int k, long long dataPtr)
    {
        cout<<"insert\n";
        if (root == nullptr)
        {
            cout<< "first root node\n";
            root = new BPlusNode(true);
            // root->is_leaf = true;
            root->size = 0;
            root->keys[0] = k;
            root->dataPtrs[0] = dataPtr;
            root->size++;
            cout<<"insertion complete\n";
        }
        else
        {
            if (root->size == 2 * degree - 1)
            {
                cout<<"reached max root size\n";
                BPlusNode *s = new BPlusNode();
                s->is_leaf = false;
                s->size = 0;
                s->children[0] = root;
                splitChild(s, 0);
                int i = 0;
                if (s->keys[0] < k)
                {
                    i++;
                }
                insertNonFull(s->children[i], k, dataPtr);
                root = s;
            }
            else
            {
                insertNonFull(root, k, dataPtr);
            }
        }
        cout <<"root: "<< root <<"\n";
    }

    void writeNodeToFile(BPlusNode *node, fstream &indexFile)
    {
        cout<<"write node to file\n";
        indexFile.write((char *)node, sizeof(BPlusNode));
        if (!node->is_leaf)
        {
            cout<<"nonleaf node\n";
            for (int i = 0; i <= node->size; i++)
            {
                writeNodeToFile(node->children[i], indexFile);
            }
        }
    }

    void writeIndexToFile(string indexFileName)
    {
        cout<<"in write index to file\n";
        fstream indexFile(indexFileName, ios::out | ios::binary);
        writeNodeToFile(root, indexFile);
        indexFile.close();
    }

    BPlusNode *readNodeFromFile(fstream &indexFile)
    {
        cout<<"read node from file\n";
        BPlusNode *node = new BPlusNode();
        indexFile.read((char *)node, sizeof(BPlusNode));
        if (!node->is_leaf)
        {
            for (int i = 0; i <= node->size; i++)
            {
                node->children[i] = readNodeFromFile(indexFile);
            }
        }
        return node;
    }

    BPlusNode *readIndexFromFile(string indexFileName)
    {
        cout<<"read index from file\n";
        fstream indexFile(indexFileName, ios::in | ios::binary);
        BPlusNode *root = readNodeFromFile(indexFile);
        indexFile.close();
        return root;
    }

    void writeDataToFile(string dataFileName, string line){
        fstream dataFile(dataFileName, ios::out | ios::binary);
        dataFile.write((char *)&line, sizeof(Record));
        dataFile.close();
    }

    vector<long long> search(BPlusNode *x, int k)
    {
        cout<<"in search\n";
        int i = 0;
        while (i < x->size && k > x->keys[i])
        {
            i++;
        }
        if (x->is_leaf)
        {
            cout<<"found leaf\n";
            vector<long long> result;
            while (i < x->size && k == x->keys[i])
            {
                result.push_back(x->dataPtrs[i]);
                i++;
            }
            return result;
        }
        else
        {
            return search(x->children[i], k);
        }
    }

    // Method to search for a key (id) in the B+ tree to find the index
    BPlusNode *search(int id)
    {
    }

    // Insert new record into the data_file and relevant information to the index_file
    void insertRecord(Record record)
    {
    }
    // Read csv file and add records to the index_file and data_file
    void createFromFile(string csvFName)
    {
    }
    // Given an id, find the relevant record and print it
    Record findRecordById(int id){
        // Use BPlusNode* search(int id) to find the index first
    };
};

BPlusNode *root = nullptr;

// struct Node
// {
//     bool isLeaf;
//     int keys[2 * d];
//     int size;
//     Node *children[2 * d + 1];
//     long long dataPtrs[2 * d];
//     Node *next;
// };

// #include <string>
// #include <vector>
// #include <string>
// #include <iostream>
// #include <sstream>
// #include <bitset>
// using namespace std;

// // Class representing a tuple from the Employee.csv file
// class Record
// {
// public:
//     int id, manager_id;
//     std::string bio, name;
//     std::string post;
//     Record() : id(0), manager_id(0) {}

//     Record(vector<std::string> fields)
//     {
//         id = stoi(fields[0]);
//         name = fields[1];
//         bio = fields[2];
//         manager_id = stoi(fields[3]);
//     }
//     void print()
//     {
//         cout << "\tID: " << id << "\n";
//         cout << "\tNAME: " << name << "\n";
//         cout << "\tBIO: " << bio << "\n";
//         cout << "\tMANAGER_ID: " << manager_id << "\n";
//     }

//     static Record readFromBinaryFile(std::ifstream &file)
//     {
//         // Implement your binary reading logic here

//         Record record;

//         // Read the id and manager_id fields from the file
//         file.read((char *)&record.id, sizeof(record.id));
//         file.read((char *)&record.manager_id, sizeof(record.manager_id));

//         // Read the length of the name string from the file
//         int nameLength;
//         file.read((char *)&nameLength, sizeof(nameLength));

//         // Resize the name string to the correct length
//         record.name.resize(nameLength);

//         // Read the name string from the file
//         file.read(&record.name[0], nameLength);

//         // Read the length of the post string from the file
//         int postLength;
//         file.read((char *)&postLength, sizeof(postLength));

//         // Resize the post string to the correct length
//         record.post.resize(postLength);

//         // Read the post string from the file
//         file.read(&record.post[0], postLength);

//         return record;
//     }
// };

// // Class representing a node in the B+ tree
// class BPlusNode
// {
// public:
//     vector<int> keys;             // Vector to store keys
//     vector<BPlusNode *> children; // Vector to store child nodes
//     bool is_leaf;                 // Boolean value indicating whether the node is a leaf node ornot
//     BPlusNode *parent;            // Pointer to the parent node
//     long fileOffset;              // if it's supposed to be an integer

//     vector<long> fileOffsets; // Add this line in your BPlusNode class

//     // Constructor to initialize the node
//     BPlusNode(bool is_leaf = false) : is_leaf(is_leaf), parent(nullptr) {}
//     // Use the Record structure to create necessary data structure to do the following :
//     // The pointers in the leaf nodes of the index must keep the addresses of their
//     // corresponding records on the created data file.
//     Record record;
// };

// // Class representing the B+ tree

// class BPlusTree
// {
// public:
//     // Necessary variables
//     BPlusNode *root; // Pointer to the root node
//     int degree;      // Degree of the B+ tree

//     // Constructor to initialize the B+ tree
//     BPlusTree(int degree = 4) : degree(degree), root(nullptr) {}

//     // Method to search for a key (id) in the B+ tree to find the index
//     BPlusNode *search(int id)
//     {
//         // If the tree is empty, return null
//         if (root == nullptr)
//         {
//             return nullptr;
//         }

//         BPlusNode *currentNode = root;

//         // While the current node is not a leaf

//         while (!currentNode->is_leaf)
//         {
//             // Go through each key in the node
//             for (size_t i = 0; i < currentNode->keys.size(); i++)
//             {
//                 // If the id is less than or equal to the current key, follow the corresponding child pointer
//                 if (id <= currentNode->keys[i])
//                 {
//                     currentNode = currentNode->children[i];
//                     break;
//                 }
//                 // If we've gone through all keys and the id is greater than all of them, follow the last child pointer
//                 if (i == currentNode->keys.size() - 1)
//                 {
//                     currentNode = currentNode->children[i + 1];
//                     break;
//                 }
//             }
//         }

//         // At this point, currentNode should be a leaf node that could potentially contain the id
//         return currentNode;
//     }

//     // Method to insert a key (id) in the B+ tree (to the index_file)
//     void insert(int id, long offset)
//     {

//         if (root == nullptr)
//         {
//             root = new BPlusNode(true);
//             root->keys.push_back(id);
//             root->fileOffsets.push_back(offset);
//             root->fileOffset = offset;

//             ofstream indexFile("EmployeeIndex.bin", ios::binary | ios::app);
//             indexFile.write((char *)&id, sizeof(int));
//             indexFile.write((char *)&offset, sizeof(long));
//             indexFile.put('\r');
//             indexFile.close();

//             return;
//         }
//         BPlusNode *node = search(id);
//         auto it = std::upper_bound(node->keys.begin(), node->keys.end(), id);
//         auto index = std::distance(node->keys.begin(), it);
//         node->keys.insert(it, id);
//         node->fileOffsets.insert(node->fileOffsets.begin() + index, offset);

//         ofstream indexFile("EmployeeIndex.bin", ios::binary | ios::app);
//         indexFile.write((char *)&id, sizeof(int));
//         indexFile.write((char *)&offset, sizeof(long));
//         indexFile.put('\r');
//         indexFile.close();

//         if (node->keys.size() > degree - 1)
//         {
//             split(node);
//         }
//     }

//     void split(BPlusNode *node)
//     {
//         BPlusNode *newNode = new BPlusNode(node->is_leaf);
//         newNode->parent = node->parent;
//         for (int i = 0; i < (degree - 1) / 2; i++)
//         {
//             newNode->keys.push_back(node->keys.back());
//             node->keys.pop_back();
//             newNode->fileOffsets.push_back(node->fileOffsets.back());
//             node->fileOffsets.pop_back();
//         }
//         if (!node->is_leaf)
//         {
//             for (int i = 0; i <= (degree - 1) / 2; i++)
//             {
//                 newNode->children.push_back(node->children.back());
//                 node->children.pop_back();
//             }
//         }
//         if (node->parent != nullptr)
//         {
//             auto it = std::find(node->parent->children.begin(), node->parent->children.end(), node);
//             node->parent->children.insert(it, newNode);
//             node->parent->keys.push_back(node->keys.back());
//             node->keys.pop_back();
//             if (node->parent->keys.size() > degree - 1)
//             {
//                 split(node->parent);
//             }
//         }
//         else
//         {
//             BPlusNode *newRoot = new BPlusNode(false);
//             newRoot->children.push_back(node);
//             newRoot->children.push_back(newNode);
//             newRoot->keys.push_back(node->keys.back());
//             node->keys.pop_back();
//             root = newRoot;
//         }
//     }

//     // Insert new record into the data_file and relevant information to the index_file
//     void insertRecord(Record record)
//     {
//         // Open the data file in binary append mode
//         // std::ofstream dataFile("EmployeeData.bin", std::ios::binary | std::ios::app);

//         ofstream dataFile("DataFile.bin", ios::binary | ios::app);
//         if (!dataFile)
//         {
//             throw std::runtime_error("Cannot open data file.");
//         }
//         dataFile.write((char *)&record, sizeof(Record));
//         streampos offset = dataFile.tellp() - streampos(sizeof(Record));
//         dataFile.close();

//         // Insert the id and offset into the B+Tree
//         insert(record.id, offset);
//     }

//     // Read csv file and add records to the index_file and data_file

//     void createFromFile(string csvFName)
//     {
//         ifstream file(csvFName);
//         if (file.is_open())
//         {
//             string line, field;
//             vector<string> fields;
//             while (getline(file, line))
//             { // Read a line
//                 stringstream s(line);
//                 fields.clear();
//                 while (getline(s, field, ','))
//                 {                            // Break line into comma delimitted fields
//                     fields.push_back(field); // Add each field to the vector
//                 }
//                 // Create a record from the fields and insert it into the B+ tree
//                 Record record(fields);
//                 insertRecord(record);
//             }
//             file.close();
//         }
//         else
//         {
//             cout << "Unable to open file " << csvFName << "\n";
//         }
//     }

//     // Given an id, find the relevant record and print it

//     Record findRecordById(int id)
//     {
//         BPlusNode *node = search(id);
//         if (node == nullptr)
//         {
//             throw std::runtime_error("Record not found.");
//         }
//         long offset = node->fileOffset;
//         cout << offset;
//         std::ifstream dataFile("EmployeeData.bin", std::ios::binary);
//         if (!dataFile)
//         {
//             throw std::runtime_error("Cannot open data file.");
//         }
//         dataFile.seekg(offset);
//         Record record = Record::readFromBinaryFile(dataFile);
//         return record;
//     };
// };
