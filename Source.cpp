#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<limits> // used to test for seek function
#include"Block.h"
using namespace std;
Block df; // directory

// Function declarations
fstream& GotoLine(fstream&, unsigned int);
void Create();
void Open();
		//void Close();
void Delete();
		//void Write();

int main() {
	bool menu = true;
	string select;
	cout << "Hi" << endl;
	cout << "This is File System_v1 created by Jonathan Ibarra." << endl;

	while (menu == true) {
		cout << "1 - Create" << endl;
		cout << "2 - Open" << endl;
		cout << "3 - Delete" << endl;
		cout << "4 - Exit" << endl;

		cout << "Enter a number: ";
		getline(cin, select);

		if (select == "1" || (select == "Create"))
			Create();

		else if ((select == "2") || (select == "Open"))
			Open();

		else if ((select == "3") || (select == "Delete"))
			Delete();

		else if ((select == "4") || (select == "Exit"))
			menu = false;

		cout << endl;
	}
}

void Create() {
	string ctype;
	string namesize;

	cout << "Create: " << endl;
	cout << "U - User Data File" << endl;
	cout << "D - Directory" << endl;

	cout << "Select a type: ";
	getline(cin, ctype);

	if (ctype == "U" || ctype == "u") {
		char ft = 'U';

		cout << "File name (up to 9 characters): ";
		getline(cin, namesize);

		while ((namesize.size() > 9) || (namesize.size() <= 0)) {
			cout << "File name (up to 9 characters): ";
			getline(cin, namesize);
		}

		unsigned short csize = 0; // because just created
		ofstream ofile(namesize);
		df.addEntry(ft, namesize, csize);
		ofile.close();
	}

	else if (ctype == "D" || ctype == "d") {
		char ft = 'D';

		cout << "Directory name (up to 9 characters): ";
		getline(cin, namesize);

		while ((namesize.size() > 9) || (namesize.size() <= 0)) {
			cout << "Directory name (up to 9 characters): ";
			getline(cin, namesize);
		}

		unsigned short csize = 0; // because just created

		df.addEntry(ft, namesize, csize);
	}

	else
		cout << "Type not detected." << endl;

	cout << endl;
}

void Open() {
	string otype;
	string fname;
	string command;
	string closecom;
	bool foundit;
	int bytelength;

	cout << "Open: " << endl;
	cout << "I - Input" << endl;
	cout << "O - Output" << endl;
	cout << "U - Update" << endl;
	cout << "C - Close" << endl;

	cout << "Select a mode: ";
	getline(cin, otype);

	cout << "Name of file: ";
	getline(cin, fname);

	while ((fname.size() > 9) || (fname.size() <= 0)) {
		cout << "Name of file: ";
		getline(cin, fname);
	}

	// search for file
	foundit = df.findEntry(fname);

	if (foundit == true) {
		// input mode
		if ((otype == "I") || (otype == "i")) {
			cout << "Input mode: " << endl;
			cout << "R - Read" << endl;
			cout << "S - Seek" << endl;

			cout << "Select a command: ";
			getline(cin, command);

			if ((command == "R") || (command == "r")) {
				cout << "Opening... " << fname << endl;
				fstream ifile(fname);
				//char reading;
				//int counter = 0;
				/*
				if (ifile.is_open()) {
					while (!ifile.eof()) {
						ifile >> reading;
						cout << reading << " ";
						counter++;
					}
				}
				*/
				//cout << endl;
				//cout << "Bytes read: " << counter << endl;

				string str2;
				int num;
				ifile.open(fname);
				cout << endl << endl;

				if (ifile.is_open())
				{
					while (ifile.good())
					{
						while (getline(ifile, str2))                // reading the file line by line
						{
							cout << str2;                   // end inner most while loop
						}                                           // end middle while loop
					}
				}
				else
				{
					cerr << "File not found";
					exit(1);
				}
			}

			else if ((command == "S") || (command == "s")) {
				int numsel;
				cout << "Seek: " << endl;
				cout << "-1 Beginning" << endl;
				cout << "0  Middle" << endl;
				cout << "1  End" << endl;

				cout << "Select a location: ";
				cin >> numsel;

				fstream ffile(fname);
				if (numsel == -1) {
					cout << "Beginning: " << endl;
					GotoLine(ffile, 1);
					string linee;
					ffile >> linee;

					cout << linee;
					cin.get();
				}

				else if (numsel == 0) {
					cout << "Middle: " << endl;
					GotoLine(ffile, 2);
					string linee;
					ffile >> linee;

					cout << linee;
					cin.get();
				}

				else if (numsel == 1) {
					cout << "End: " << endl;
					GotoLine(ffile, 3);
					string linee;
					ffile >> linee;

					cout << linee;
					cin.get();
				}

				else {
					cout << "Location not recognized." << endl;
				}
			}
		}

		// output mode
		else if ((otype == "O") || (otype == "o")) {
			cout << "Output mode: " << endl;
			cout << "Writing to file is only available..." << endl;

			ofstream oofile(fname);
			string usertypes;

			cout << "Press enter to end." << endl;
			getline(cin, usertypes);

			oofile << usertypes << endl;
			oofile.close();
		}

		else if ((otype == "U") || (otype == "u")) {
			cout << "Update mode: " << endl;
			cout << "Select a command. " << endl;
			cout << "R - Read" << endl;
			cout << "S - Seek" << endl;
			cout << "W - Write" << endl;

			getline(cin, command);

			if ((command == "R") || (command == "r")) {
				cout << "Read: " << endl;
				ifstream ifile(fname);
				string reading;
				while (getline(ifile, reading)) {
					istringstream iss(reading);
					string a;
					if (!(iss >> a))
						break;
				}
			}

			else if ((command == "S") || (command == "s")) {
				int numsel;
				cout << "Seek: " << endl;
				cout << "-1 Beginning" << endl;
				cout << "0  Middle" << endl;
				cout << "1  End" << endl;

				cout << "Select a location: ";
				cin >> numsel;

				fstream ffile(fname);
				if (numsel == -1) {
					cout << "Beginning: " << endl;
					GotoLine(ffile, 1);
					string linee;
					ffile >> linee;

					cout << linee;
					cin.get();
				}

				else if (numsel == 0) {
					cout << "Middle: " << endl;
					GotoLine(ffile, 2);
					string linee;
					ffile >> linee;

					cout << linee;
					cin.get();
				}

				else if (numsel == 1) {
					cout << "End: " << endl;
					GotoLine(ffile, 3);
					string linee;
					ffile >> linee;

					cout << linee;
					cin.get();
				}
			}

			else if ((command == "W") || (command == "w")) {
				cout << "Write: " << endl;

				ofstream oofile(fname);
				string usertypes;

				cout << "Press enter to end." << endl;
				getline(cin, usertypes);

				oofile << usertypes << endl;
				oofile.close();
			}
		}
	}

	else
		cout << "File does not exist." << endl;

	cout << endl;
}

// I tried other options to seek, trying out how other coders loop through list 
// stackoverflow.com/questions/5207550/in-c-is-there-a-way-to-go-to-a-specific-line-in-a-text-file?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa
fstream& GotoLine(fstream& file, unsigned int num) {
	file.seekg(ios::beg);
	for (int i = 0; i < num - 1; ++i) {
		file.ignore(numeric_limits < streamsize>::max(), '\n');
	}
	return file;
}

void Delete() {
	string delfile;
	cout << "Delete:" << endl;
	cout << "Name a file you want to delete: ";
	getline(cin, delfile);

	while ((delfile.size() > 9) && (delfile.size() <= 0)) {
		cout << "Name a file you want to delete: ";
		getline(cin, delfile);
	}

	df.removeEntry(delfile);
}