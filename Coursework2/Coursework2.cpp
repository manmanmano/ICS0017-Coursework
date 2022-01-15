#include <iostream>
#include <string.h>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"
#include "Datastructure.h"
using namespace std;

// Mariano D'Angelo --> Struct 3 = HEADER_A **GetStruct3(int iItem, int nItems), Item10 & DATE3

int main() {

	// numbers in the comments refer to test cases
	
	// 1 
	cout << "##################  TEST CASE 1 ##################" << endl << endl << endl;
	DataStructure ds;
	// 2 
	cout << "##################  TEST CASE 2 ##################" << endl << endl << endl;
	cout << "Inserting items into the datastructure..." << endl << endl << endl;
	for (int i = 0; i < 10; i++) {
		ITEM10* item = (ITEM10*)GetItem(10);
		ds += item;
	}
	// 3
	cout << "##################  TEST CASE 3 ##################" << endl << endl << endl;
	cout << "The original datastructure contains:" << endl;
	cout << endl << ds << endl << endl;
	// 4
	cout << "##################  TEST CASE 4 ##################" << endl << endl << endl;
	cout << "There are " << ds.GetItemsNumber() << " items in the datastructure." << endl << endl << endl;
	// 5 - 6
	cout << "################  TEST CASE 5 - 6 ################" << endl << endl << endl;
	cout << "Finiding requested items..." << endl << endl;
	cout << "Results in finding requested items:" << endl << endl;
	char itemsToFind[][20] = { "Light Cyan", "X X" };
	for (int i = 0, j = 1; i < 2; i++, j++) {
		cout << j << ") ";
		ds.GetItem(itemsToFind[i]);
	}
	cout << endl << endl;
	// 7
	cout << "##################  TEST CASE 7 ##################" << endl << endl << endl;
	cout << "Creating copy of the original datastructure..." << endl << endl;
	DataStructure dscpy = ds;
	cout << "The copy of the original datastructure contains:" << endl << endl;
	cout << dscpy << endl << endl;
	// 8
	cout << "##################  TEST CASE 8 ##################" << endl << endl << endl;
	cout << "Removing items from the datastructure..." << endl;
	cout << endl << "The original datastructure after removal of items contains:" << endl << endl;
	char itemsToRemove[][20] = { "Banana Mania", "Persian Green", "Vegas Gold"};
	for (int i = 0; i < 3; i++) {
		ds -= itemsToRemove[i];
	}
	cout << ds << endl << endl;
	// 9
	cout << "##################  TEST CASE 9 ##################" << endl << endl << endl;
	cout << "Comparing datastructures..." << endl << endl;
	cout << (ds == dscpy) << endl << endl << endl;
	// 10
	cout << "##################  TEST CASE 10 ##################" << endl << endl << endl;
	cout << "Trying to opening file..." << endl << endl;
	char filename[] = "C:\\Users\\Antonio\\Documents\\ds.bin";
	ds.Write(filename);
	// 11
	cout << endl << "##################  TEST CASE 11 ##################" << endl << endl;
	cout << endl << "Creating datastructure from file..." << endl;
	DataStructure dsfile(filename);
	cout << endl << "The datastructure generated from the file contains:" << endl << endl;
	cout << dsfile << endl;
	cout << "Comparing datastructures..." << endl << endl;
	cout << (ds == dsfile) << endl << endl << endl;
	// 12
	cout << "##################  TEST CASE 12 ##################" << endl << endl;
	cout << endl << "Assigning copy datastructure to file datastructure..." << endl;
	cout << endl << "The datastructure read from file after assignment contains:" << endl << endl;
	dsfile = dscpy;
	cout << dsfile << endl;
	return 0;
}