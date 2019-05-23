// reminder might need a function to go to a certain directory
// multiple directories in a block
#include<iostream>
#include<string>
#include<fstream>
using namespace std;

#ifndef BLOCK_H
#define BLOCK_H

class Block {
protected:
	// Information needed for an entry to a block
	struct entry {
		char type; // 1 byte
		string ename; // 9 bytes
		int link; // 4 bytes
		unsigned short esize; // 2 bytes
	};

	struct bnode {
		// 16 bytes
		bnode *next;
		bnode *prev;
		int free; // number of first available entry in a block
		bnode *filler;

		// array of entries
		entry entries[31] = {};
	};

	bnode *head;
	bnode *feet;

	struct fnode {
		fnode *next;
		fnode *prev;
		int bytes;
	};

	fnode *first;
	fnode *last;

	int totalblocks = 1; // counter for blocks used, 1 block reserved for root
	const int maxblocks = 500; // maximum number of blocks for file system
							   // remember 16 bytes used for: fwd, back, free, and filler
							   // remember 496 bytes used for: entries
	const int maxentry = 31;
	int directorylocation = -100; // used for removeEntry function
	int entrylocation = -100; // used for removeEntry function

public:
	Block();
	~Block();
	void addDirectory();
	void addFile();
	bool fullBlock();
	bool emptyFilelist();

	bool findEntry(string);
	void addEntry(char, string, unsigned short);
	void removeEntry(string);
	void printData();
};

#endif

Block::Block() {
	// on boot up we want to create a root directory block
	bnode *n = new bnode;
	n->next = nullptr;
	n->prev = nullptr;
	n->free = 0;
	n->filler = nullptr;

	// initialize all entries in root to type F
	for (int i = 0; i < maxentry; i++)
		n->entries[i].type = 'F';

	head = n;
	feet = n;

	// initialize fnode first, last
	first = nullptr;
	last = nullptr;
}

Block::~Block() {
	//clearBlock();
}

bool Block::fullBlock() {
	if (totalblocks == maxblocks)
		return true;
	else
		return false;
}

bool Block::emptyFilelist() {
	return first == nullptr;
}

void Block::addEntry(char ty, string na, unsigned short si) {
	// check if there are available entries for current block
	// if available, then find first available entry
	// if not available, then entry cannot be added

	if (!fullBlock()) {
		bnode *p = feet;
		char a = ty;
		string b = na;
		unsigned short c = si;
		int freeEntry = 0; // counter to count available entries in current block
		int hold; // hold the location of first available entry

		// check if a file or directory has the same name then overwrite
		for (int i = 0; i < maxentry; i++) {
			if ((p->entries[i].ename == b) && (p->entries[i].type == 'U') && (a == 'U')) {
				cout << "Overwriting..." << endl;
				p->entries[i].type = a; // storing type
				p->entries[i].ename = b; // storing name
				p->entries[i].esize = c; // storing size (when created it's just 0)
				p->entries[i].link = i; // storing link (where the file is the entries array)
				return;
			}
		}

		for (int i = 0; i < maxentry; i++) {
			if (p->entries[i].type == 'F')
				freeEntry++;
		}

		cout << "There is/are " << freeEntry << " available entry/entries." << endl;

		// if freeEntry > 0 then you can add an entry to this block
		if (freeEntry > 0) {
			for (int i = 0; i < maxentry; i++) {
				if (p->entries[i].type == 'F') {
					hold = i;
					break;
				}
			}

			cout << hold << " is the first available entry." << endl;
		}

		else
			hold = -1000; // placeholder

		if ((hold >= 0) && (hold < maxentry)) {
			// check entry type then build accordingly
			if (a == 'U') {
				cout << "Adding a User Data File." << endl;
				p->entries[hold].type = a; // storing type
				p->entries[hold].ename = b; // storing name
				p->entries[hold].esize = c; // storing size (when created it's just 0)
				p->entries[hold].link = hold; // storing link (where the file is the entries array)
				addFile();
				cout << "File added." << endl;
				totalblocks++;
			}

			else if (a == 'D') {
				cout << "Adding a Directory." << endl;
				p->entries[hold].type = a; // storing type
				p->entries[hold].ename = b; // storing name
				p->entries[hold].esize = c; // storing size (when created it's just 0)
				p->entries[hold].link = hold; // storing link (where the directory is the entries array)
				addDirectory();
				cout << "Directory added." << endl;
				totalblocks++;
			}

			else
				cout << "Error when checking entry type." << endl;

			printData();
		}

		else
			cout << "There are no available entries." << endl;
	}
}

void Block::addDirectory() {
	bnode *n = new bnode;
	n->next = nullptr;
	n->prev = feet;
	feet->next = n;
	feet = feet->next;
	n->free = 0;
	n->filler = nullptr;

	for (int i = 0; i < maxentry; i++) {
		n->entries[i].ename = "";
		n->entries[i].esize = 0;
		n->entries[i].link = 0;
		n->entries[i].type = 'F';
	}
}

void Block::addFile() {
	if (emptyFilelist()) {
		fnode *n = new fnode;
		n->next = nullptr;
		n->prev = nullptr;
		n->bytes = 0;

		first = n;
		last = n;
	}

	else {
		fnode *n = new fnode;
		n->next = nullptr;
		n->prev = last;
		last->next = n;
		last = last->next;
		n->bytes = 0;
	}
}

void Block::printData() {
	int countBlocks = 0; // count how many directories exist
	bnode *start = head; // starting point

	while (start != nullptr) {
		countBlocks++;
		start = start->next;
	}

	start = head; // start over
	int dircounter = 0;
	int filecounter = 0;

	cout << "Total directories in memory: " << countBlocks << endl;
	cout << "Total Blocks variable: " << totalblocks << endl;

	for (int i = 0; i < countBlocks; i++) {
		for (int j = 0; j < maxentry; j++) {
			if (start->entries[j].type == 'D') {
				dircounter++;
				cout << start->entries[j].ename << "  ";
			}

			else if (start->entries[j].type == 'U') {
				filecounter++;
				cout << start->entries[j].ename << "  ";
			}
				
		}

		cout << endl;
		cout << "Directory #" << i << " has: " << endl;
		cout << dircounter << " directory/directories" << endl;
		cout << filecounter << " file/files" << endl;

		// reset counters
		dircounter = 0;
		filecounter = 0;
		start = start->next;
	}
}

bool Block::findEntry(string en) {
	bnode *p = head;
	int dCounter = 0; // directory counter
	
	while (p != nullptr) {
		for (int i = 0; i < maxentry; i++) {
			if ((p->entries[i].type == 'U') && (p->entries[i].ename == en)) {
				cout << p->entries[i].ename << " found!" << endl;
				cout << "Location:" << endl;
				cout << "Directory #" << dCounter << endl;
				cout << "Entry #" << i << endl;
				directorylocation = dCounter;
				entrylocation = i;
				return true;
			}
		}
		p = p->next;
		dCounter++;
	}

	return false;
}

void Block::removeEntry(string e) {
	string hold = e; // hold the name of the file to delete
	bool entryexist = findEntry(hold);

	if (entryexist == true) {
		cout << endl;

		cout << "Entry exists and proceeding to destroy file." << endl;
		bnode *z = head;
		
		// move to exact directory location
		/*
		while (z != nullptr) {
			for (int i = 0; i < maxentry; i++) {
				if ((z->entries[i].type == 'U') && (z->entries[i].ename == hold)) {
					break;
				}
			}

			z = z->next;
		}*/

		// remove exact location of entry (change to 'F')
		z->entries[entrylocation].type = 'F';
/*
		// remove file from linked list
		// only one node // && (last->next == nullptr) && (last->prev == nullptr)
		if ((first == last) && (first->next == nullptr) && (first->prev == nullptr)) {
			delete first;
			first = nullptr;
			last = nullptr;

			cout << "No files exist in this directory." << endl;
		}

		// multiple nodes
		else {
			fnode *p = last;
			fnode *q = last->prev;
			q->next = nullptr;
			last = last->prev;
			delete p;

			cout << "File deleted. " << endl;
		}*/
	}

	directorylocation = -100;
	entrylocation = -100;
}