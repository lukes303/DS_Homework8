//
// ========================================
// HW8: Implement Hash Table with the 
//      Coalesced Chaining Technique 
// ========================================
// 
// In this assignment, we will implement a 
// hash table which uses coalesced chaining 
// to deal with collisions. The chains should 
// be implemented using singly linked list. 
// 
// The Node class (used to construct chain) 
// has been defined to you. Do not modify it. 
// 
// *********************************************
// Note this Node class is slightly different 
// from the ones we have been working with.
// 
// When constructing a chain, instead of using 
// a pointer "p_next" to hold address of the 
// next node, it uses an integer "idx_next" to 
// hold position of the next node in the table. 
// 
// For example, if an object's idx_next = 2, 
// it means the next node on the chain is 
// stored at table[2]. If idx_next = -1, 
// it means the object is tail of the chain. 
// *********************************************
// 
// Your task is to implement a HashTable class 
// based on the following template and instructions. 
// 
// 


#include <iostream>
using namespace std;


// ------------------------------------
// The Node class is given to you. 
// Do not modify it, except that you 
// can create a "copy" function 
// to facilitate implementation. 
// (See more explanations in the Add 
// function of the HashTable class.)
// ------------------------------------
class Node {
private:
	int key;
	int idx_next;
public:
	void Set_key(int x);
	int Get_key();
	void Set_index(int x);
	int Get_index();
	Node();
};
Node::Node() {
	key = -1;
	idx_next = -1;
}
void Node::Set_key(int x) {
	key = x;
}
int Node::Get_key() {
	return key;
}
void Node::Set_index(int x) {
	idx_next = x;
}
int Node::Get_index() {
	return idx_next;
}


// -----------------------------------------------
// ----------------- Your Task -----------------
// -----------------------------------------------
// We will implement a hash table which addresses 
// collisions using coalesced chaining. 
// 
// We will construct the table as an array of 
// Node objects. Objects colliding at a cell 
// are placed on a chain, connected one after 
// another through the "idx_next" variable. 
// (See how it is used in the above instructions.) 
// 
// In this hw, let us always add a new collided 
// object to the tail of a chain. For example, if 
// table[5] is the current tail and the new object 
// (which collide at the same cell as table[5]) 
// is to be stored at table [7], then table[5]'s 
// "idx_next" should equal to 7. 
// 
// When the table is full, you should resize it 
// (by doubling its size) and rehash objects 
// from the existing table to the new table. 
// You need to rehash objects based on the order 
// of their indices in the table i.e., first 
// rehash table[0], then table[1], then table[2]... 
// 
// This template does not specify how you should 
// resize the table. There are many options e.g., 
// design a resize function and call it, or 
// embed a resize-rehash process inside the Add() 
// function. Make your own choice, but do not 
// alert the input/output of defined functions. 
// 
// Finally, we will use a special "Empty" Object 
// with SID = -1 to indicate a cell is empty. 
// For example, if table[2].Get_key() returns -1, 
// it means table[2] is empty. 
// 
// The HashTable class is structured as follows. 
// Do not modify the structure but you can add 
// functions in two places: 
// (1) a "copy" function for the Node class. 
// (2) some function(s) for resizing and rehashing
// 
// Please complete the class implementation.
// -----------------------------------------------
// 
class HashTable {

private:

	// This is size of the hash table. 
	int size;

	// This is the hash function "h". 
	// It takes "key" as input and returns 
	// a hash code h(key) = key % divisor.
	int hash(int key);

	// This is a pointer to the hash table.  
	// Note it is a pointer of objects. 
	Node* table;


public:

	// This function creates a hash table 
	// (through dynamic memory allocation)
	// and initializes all cells to NULL.
	// 
	// It should create a table of the 
	// smallest necessary size N, which 
	// depends on the input divisor 
	// (of the modulo hash function).
	// 
	// This function should also update 
	// the private variable "size" to N. 
	void CreateTable(int divisor);


	// This function searchs for a student 
	// whose SID = key. If a student is found, 
	// return index of the student in the table. 
	// Otherwise, return -1.
	int Search(int key);

	//Resize function
	void Resize();

	// This function adds a student to the hash 
	// table. Note the input is an object instead
	// of a pointer. We cannot directly assign 
	// an object to another, but have to assign 
	// the member variables one by one. 
	// (You can create a "copy" function for the 
	// Node class to facilitate object assignment, 
	// but the essential process should be the same.) 
	// 
	// In addition, the add function should apply 
	// linear probing to look for an empty cell 
	// for a collided object with key = t. Probing 
	// should start from table[h(t)].(Not the 
	// tail of the chain.)  
	void Add(Node temp);


	// This function removes a student whose 
	// SID = key from the table. If the student 
	// is not in the table, it does nothing. 
	void Remove(int key);


	// This function returns table size. 
	// Specifically, it returns value of 
	// the private variable "size". 
	int Get_Size();


	// This is the constructor. It is defined 
	// for you. No need to modify it. 
	HashTable();


	// This is the 1st print function. 
	// It will print all SID's in the table 
	// from table[0] to table[N-1]. 
	// It is defined for you and will be 
	// used to examine the correctness of 
	// your created/updated hash table. 
	// Do not modify it. 
	void PrintTable();


	// This is the 2nd print function. 
	// It will print all SID's on the chain 
	// starting at table[h(key)]. 
	// It is defined for you and will be 
	// used to examine the correctness of 
	// your created/updated chains. 
	// Do not modify it. 
	void PrintChain(int key);
};

//Hash
int HashTable::hash(int key){
	return key % size;
}

//Create Table
void HashTable::CreateTable(int divisor){
	size = divisor;
	table = new Node[size];

	//Assign nodes with -1 as key to indicate empty space
	for(int i = 0; i < size; i++){
		Node temp;
		table[i] = temp;
	}
}

// Search
int HashTable::Search(int key) {
    int index = hash(key); // Calculate the initial index using the hash function
    int originalIndex = index;

    //Linear probing to search for the key
    while (table[index].Get_key() != key) {
		//If we get an empty slot or have checked all slots without finding the key, it means the key is not in the table.
        if (table[index].Get_key() == -1 || index == originalIndex) {
            return -1;
        }
		//Move to the next slot using modulo to wrap around
        index = (index + 1) % size; 
    }

    // We found the key at the current index
    return index;
}

//Resize
void HashTable::Resize() {
    int newSize = size * 2; // Double the size
    Node* newTable = new Node[newSize];

    // Rehash all existing elements into the new table
    for (int i = 0; i < size; ++i) {
        if (table[i].Get_key() != -1) {
            int newIndex = hash(table[i].Get_key());
            while (newTable[newIndex].Get_key() != -1) {
                newIndex = (newIndex + 1) % newSize; // Linear probing in the new table
            }
            newTable[newIndex] = table[i];
        }
    }

    // Clean up the old table and update the size
    delete[] table;
    table = newTable;
    size = newSize;
}

// Add
void HashTable::Add(Node temp) {
    int key = temp.Get_key();
    int index = hash(key); // Calculate the initial index using the hash function
    int originalIndex = index;

    // Linear probing to find an empty slot
    while (table[index].Get_key() != -1) {
        index = (index + 1) % size; // Move to the next slot using modulo to wrap around
        if (index == originalIndex) {
            // If we have checked all slots and couldn't find an empty one, then the table is full. Call the Resize function.
            Resize();
            //continue adding the new Node
            index = originalIndex;
        }
    }

    // We found an empty slot, so add the Node
    table[index] = temp;
}

//Remove
void HashTable::Remove(int key) {
    int index = hash(key); // Calculate the initial index using the hash function

    Node* currentNode = &table[index];
    Node* prevNode = nullptr;

    // Traverse the chain at the given index to find the key
    while (currentNode != nullptr && currentNode->Get_key() != key) {
        prevNode = currentNode;
        currentNode = &table[currentNode->Get_index()];
    }

    if (currentNode != nullptr && currentNode->Get_key() == key) {
        // Key found in the chain, remove it
        if (prevNode == nullptr) {
            // If it's the head of the chain, update the table slot
            int nextIndex = currentNode->Get_index();
            if (nextIndex != -1) {
                table[index] = table[nextIndex];
                table[nextIndex].Set_key(-1);
            } else {
                table[index].Set_key(-1); // If it's the only element in the chain
            }
        } else {
            // If it's not the head, simply remove it by updating the previous node's idx_next
            prevNode->Set_index(currentNode->Get_index());
        }
    }
}

//Size
int HashTable::Get_Size(){
	return size;
}

// This is the constructor. 
// It is defined for you. 
HashTable::HashTable() {
	table = NULL;
	size = 0;
}


// This is the 1st print function. 
// It is defined for you.
// Do not modify it.
void HashTable::PrintTable() {
	for (int i = 0; i < size; i++) {
		cout << table[i].Get_key() << '\n';
	}
}


// This is the 2nd print function. 
// It is defined for you.
// Do not modify it.
void HashTable::PrintChain(int key_chain) {
	int temp = hash(key_chain);
	while (table[temp].Get_key() != -1) {
		cout << table[temp].Get_key() << '\n';
		temp = table[temp].Get_index();
	}
}


// --------------
// Main Function 
// --------------
int main()
{
	int mode, temp;

	// This key is used as a search key to 
	// test the search and remove functions. 
	int key;

	// This key is used by the 2nd print function.
	int key_chain;

	// This divisor is used by the hash function. 
	// It is also used by the CreateTable() function 
	// to automatically determine the table size. 
	int divisor;

	// We don't need dynamically memory allocation 
	// for taking input students. Do you know why?  
	Node Student;

	HashTable x;

	cin >> mode >> key >> key_chain >> divisor;

	x.CreateTable(divisor);

	while (cin >> temp) {

		Student.Set_key(temp);

		x.Add(Student); 
	}

	// Mode 0: test the "Add" function and 
	// the resizing and rehashing processes. 
	// Keep in mind we will test two scenarios:
	// (i) table is not full after the above adding
	// (ii) table becomes full during adding (need resizing)
	if (mode == 0) {
		x.PrintTable();
	}
	// Mode 1: test if your chains are correct
	else if (mode == 1) {
		x.PrintChain(key_chain);	
	}
	// Mode 2: test the "Search" function 
	else if (mode == 2) {
		cout << x.Search(key);
	}
	// Mode 3: test the "Remove" function 
	// We will test both the updated table 
	// and the updated chain. 
	else if (mode == 3) {
		x.Remove(key);
		x.PrintTable();
		x.PrintChain(key);
	}

	return 0;
}

