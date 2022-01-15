#include <iostream>
#include <string.h>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"
using namespace std;

class DataStructure {

private:
	
	HEADER_A** ppStruct = new HEADER_A * [26]();

public:

	// functions are numbered with the number of its test case
	
	DataStructure(); // 1

	~DataStructure(); // 1

	void operator+=(ITEM10* pNewItem); // 2

	friend ostream& operator<<(ostream& ostr, const DataStructure& str); // 3

	int GetItemsNumber(); // 4

	char* GetItem(char* pID); // 5 / 6

	DataStructure(const DataStructure& Original); // 7

	void operator-=(char* pID); // 8

	int operator==(DataStructure& Other); // 9

	void Write(char* pFilename);  // 10

	DataStructure(char* pFilename); // 11

	DataStructure& operator=(const DataStructure& Right); //12

};