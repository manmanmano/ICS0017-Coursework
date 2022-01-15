#include <iostream>
#include <string.h>
#include <string>
#include <fstream>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"
#include "Datastructure.h"
using namespace std;

// this function is used to find an item's ID second word first letter
char FindSecondChar(char* pNewItemID) {
	char* pSecondId = (char*)' ', secondChar;
	char* pSpace = strchr(pNewItemID, ' '); // find the second word of ID
	if (!pSpace) {
		return 0;
	}
	pSecondId = pSpace + 1;
	secondChar = *(pSecondId);
	return secondChar;
}

// functions are numbered with the number of their respective test case

// 1 - create empty datastructure
DataStructure::DataStructure() { cout << "Created empty datastructure." << endl << endl << endl; }

// 1 - destroy datastracture
DataStructure::~DataStructure() {
	for (int i = 0; i < 26; i++) {
		for (HEADER_A* pHeader = *(ppStruct + i); pHeader; ) {
			for (ITEM10* pItem = (ITEM10*)pHeader->pItems; pItem; pItem = pItem->pNext) {
				if (!pItem->pNext) {
					delete pItem;
					break;
				}
				if (pItem) {
					delete pItem;
				}
			}
			if (!pHeader->pNext) {
				delete pHeader;
				break;
			}
			if (pHeader) {
				HEADER_A* temp = pHeader->pNext;
				delete pHeader;
				pHeader = temp;
			}
		}
	}
	if (ppStruct) {
		delete[] *ppStruct;
		delete[] ppStruct;
	}
}

// 2 - insert into datastructure
void DataStructure::operator+=(ITEM10* pNewItem) { 
	char firstChar = *(pNewItem->pID), secondChar = FindSecondChar(pNewItem->pID);
	try {
		for (HEADER_A* pHeader = *((ppStruct + firstChar) - 65); pHeader; pHeader = pHeader->pNext) {
			for (ITEM10* pItem = (ITEM10*)pHeader->pItems; pItem; pItem = pItem->pNext) {
				if (!strcmp(pItem->pID, pNewItem->pID)) {
					throw "Could not insert item, because it already existed.";
				}
			}
		}
	}
	catch (const char* e) {
		cout << "\n!!! ERROR - " << e << " Presented ID was '" << pNewItem->pID << "'." << endl;
		return;
	}
	HEADER_A* pHeader = *((ppStruct + firstChar) - 65), * pNewHeader;
	if (!*((ppStruct + firstChar) - 65)) {
		pNewHeader = new HEADER_A;
		pNewHeader->cBegin = secondChar;
		pNewHeader->pItems = pNewItem;
		*((ppStruct + firstChar) - 65) = pNewHeader;
		pNewHeader->pNext = 0;
		return;
	}
	else {
		for (; pHeader; pHeader = pHeader->pNext) {
			if (secondChar == pHeader->cBegin) {
				pNewItem->pNext = (ITEM10*)pHeader->pItems;
				pHeader->pItems = pNewItem;
				break;
			}
			else if (secondChar < pHeader->cBegin) {
				pNewHeader = new HEADER_A;
				pNewHeader->cBegin = secondChar;
				pNewHeader->pItems = pNewItem;
				*((ppStruct + firstChar) - 65) = pNewHeader;
				pNewHeader->pNext = pHeader;
				break;
			}
			else {
				if (!pHeader->pNext) {
					pNewHeader = new HEADER_A;
					pNewHeader->cBegin = secondChar;
					pNewHeader->pItems = pNewItem;
					pHeader->pNext = pNewHeader;
					pNewHeader->pNext = 0;
					break;
				}
				else if (secondChar > pHeader->cBegin && secondChar < pHeader->pNext->cBegin) {
					pNewHeader = new HEADER_A;
					pNewHeader->cBegin = secondChar;
					pNewHeader->pItems = pNewItem;
					pNewHeader->pNext = pHeader->pNext;
					pHeader->pNext = pNewHeader;
					break;
				}
				else {
					continue;
				}
			}
		}
		return;
	}
} 

// 3 - print datastructure
ostream& operator<<(ostream& ostr, const DataStructure& str) {
	int j = 1;
	try {
		if (!str.ppStruct) { // if the data structure is empty return the empty data structure
			throw "The chosen data structure is emtpy!";
		}
		for (int i = 0; i < 26; i++) {
			for (HEADER_A* pHeader = *(str.ppStruct + i); pHeader; pHeader = pHeader->pNext) {
				for (ITEM10* pItem = (ITEM10*)pHeader->pItems; pItem; pItem = pItem->pNext, j++) {
					ostr << j << ")" << " " << pItem->pID << " " << pItem->Code << " " << pItem->Date.Day << " " << pItem->Date.pMonth << " " << pItem->Date.Year << endl;
				}
			}
		}
	}
	catch (const char* e) {
		cout << "!!! ERROR - " << e << endl;
		return ostr;
	}
	return ostr;
}

// 4 - count items in datastructure
int DataStructure::GetItemsNumber() {
	int itemsCounter = 0;
	for (int i = 0; i < 26; i++) {
		for (HEADER_A* pHeader = *(ppStruct + i); pHeader; pHeader = pHeader->pNext) {
			for (ITEM10* pItem = (ITEM10*)pHeader->pItems; pItem; pItem = pItem->pNext) {
				itemsCounter++;
			}
		}
	}
	return itemsCounter;
} 

// 5 / 6 - get item with specified ID, return 0 if it does not exist
char* DataStructure::GetItem(char* pID) {
	char firstChar = *(pID), secondChar = FindSecondChar(pID);
	try {
		for (HEADER_A* pHeader = *((ppStruct + firstChar) - 65); pHeader; pHeader = pHeader->pNext) {
			if (secondChar == pHeader->cBegin) {
				for (ITEM10* pItem = (ITEM10*)pHeader->pItems; pItem; pItem = pItem->pNext) {
					if (!strcmp(pID, pItem->pID)) {
						cout << "Item '" << pItem->pID << "' exists." << endl;
						return pItem->pID;
					}
				}
			}
		}
		throw "Item does not exist.";
	}
	catch (const char* e) {
		cout << e << " Presented ID was '" << pID << "'." << endl;
		return 0;
	}
}

// 7 - make a copy of the datastructure
DataStructure::DataStructure(const DataStructure& Original) {
	for (int i = 0; i < 26; i++) {
		for (HEADER_A* pHeader = *(Original.ppStruct + i); pHeader; pHeader = pHeader->pNext) {
			ITEM10* pItemCopy = new ITEM10;
			for (ITEM10* pItem = (ITEM10*)pHeader->pItems; pItem; pItem = pItem->pNext) {
				pItemCopy->Code = pItem->Code;
				pItemCopy->Date = pItem->Date;
				pItemCopy->pID = pItem->pID;
				pItemCopy->pNext = 0;
				*this += pItemCopy;
			}
		}
	}
}

// 8 - delete element with the specified ID
void DataStructure::operator-=(char* pID) {
	char firstChar = *(pID), secondChar = FindSecondChar(pID), * afterSpace = strchr(pID, ' ');;
	try {
		if (strlen(pID) < 3 || !strstr(pID, " ") || !strpbrk(afterSpace + 1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ")) {
			throw "Item does not follow the formatting rules, not deleted.";
		} 
		if (firstChar < 'A' || firstChar > 'Z' || secondChar < 'A' || secondChar > 'Z') {
			throw "Item does not follow the formatting rules, not deleted.";
		}
	}
	catch (const char* e) {
		cout << "!!! ERROR - " << e << " Presented ID was '" << pID << "'." << endl << endl;
		return;
	}
	try {
		for (HEADER_A* pHeader = *((ppStruct + firstChar) - 65), *pPrevHeader = 0; pHeader; pPrevHeader = pHeader, pHeader = pHeader->pNext) {
			if (secondChar == pHeader->cBegin) {
				for (ITEM10* pItem = (ITEM10*)pHeader->pItems, *pPrevItem = 0; pItem; pPrevItem = pItem, pItem = pItem->pNext) {
					if (!strcmp(pID, pItem->pID)) {
						if (!pPrevItem && !pItem->pNext) {
							if (pHeader == *((ppStruct + firstChar) - 65) && pHeader->pNext) {
								*((ppStruct + firstChar) - 65) = pHeader->pNext;
								delete(pItem);
								delete(pHeader);
								return;
							}
							else if (pHeader == *((ppStruct + firstChar) - 65) && !pHeader->pNext) {
								*((ppStruct + firstChar) - 65) = 0;
								delete(pItem);
								delete(pHeader);
								return;
							}
							else if (pHeader != *((ppStruct + firstChar) - 65) && !pHeader->pNext) {
								pPrevHeader->pNext = 0;
								delete(pItem);
								delete(pHeader);
								return;
							}
							else {
								pPrevHeader->pNext = pHeader->pNext;
								delete(pItem);
								delete(pHeader);
								return;
							}
						}
						else if (pPrevItem && !pItem->pNext) {
							pPrevItem->pNext = 0;
							delete(pItem);
							return;
						}
						else if (!pPrevItem && pItem->pNext) {
							pHeader->pItems = pItem->pNext;
							delete(pItem);
							return;
						}
						else {
							pPrevItem->pNext = pItem->pNext;
							delete(pItem);
							return;
						}
					}
				}
			}
		}
		throw "Item could not be deleted, because it did not exist.";
	}
	catch (const char* e) {
		cout << "!!! ERROR - " << e << " Presented ID was '" << pID << "'." << endl << endl;
		return;
	}
}

// 9 - compare the initial datastructure with the copied structure
int DataStructure::operator==(DataStructure& Other)  {
	if (Other.ppStruct == ppStruct) {
		cout << "The compared datastructures are the same!" << endl;
		cout << "Exit code is: ";
		return 0;
	}
	int itemsCounter = 0;
	for (int i = 0; i < 26; i++) {
		for (HEADER_A* pHeader = *(ppStruct + i); pHeader; pHeader = pHeader->pNext) {
			for (ITEM10* pItem = (ITEM10*)pHeader->pItems; pItem; pItem = pItem->pNext) {
				itemsCounter++;
			}
		}
	}
	if (itemsCounter != Other.GetItemsNumber()) {
		cout << "The datastructures are not the same." << endl;
		cout << "The original datastructure contains " << itemsCounter << " items." << endl;
		cout << "The other datastructure contains " << Other.GetItemsNumber() << " items." << endl;
		cout << "Exit code is: ";
		return 1;
	}
	int matchCounter = 0;
	for (int i = 0; i < 26; i++) {
		for (HEADER_A* phOriginal = *(ppStruct + i), * phOther = *(Other.ppStruct + i); phOriginal; phOriginal = phOriginal->pNext, phOther = phOther->pNext) {
			for (ITEM10* piOriginal = (ITEM10*)phOriginal->pItems; piOriginal; piOriginal = piOriginal->pNext) {
				for (ITEM10* piOther = (ITEM10*)phOther->pItems; piOther; piOther = piOther->pNext) {
					if (!strcmp(piOriginal->pID, piOther->pID) && piOriginal->Code == piOther->Code && 
						piOriginal->Date.Day == piOther->Date.Year && !strcmp(piOriginal->Date.pMonth, piOther->Date.pMonth)) {
						matchCounter++;
					}
				}
			}
		}
	}
	if (matchCounter == itemsCounter) {
		cout << "The compared datastructures are not the same!" << endl;
		cout << "Exit code is: ";
		return 1;
	}
	cout << "The compared datastructures are the same!" << endl;
	cout << "Exit code is: ";
	return 0;
}

// 10 - write initial datastructure to file
void DataStructure::Write(char* pFilename) {
	fstream file;	
	file.open(pFilename, fstream::out | fstream::binary);
	try {
		if (!file.good()) {
			throw "Error in opening the requested file.";
		}
	}
	catch (const char* e) {
		cout << "ERROR - " << e << endl;
		return;
	}
	try {
		if (!this->ppStruct) {
			throw "The selected datastructure is empty!";
		}
	}
	catch (const char* e) {
		cout << "ERROR - " << e << endl;
		return;
	}
	string strData;
	for (int i = 0; i < 26; i++) {
		for (HEADER_A* pHeader = *(ppStruct + i); pHeader; pHeader = pHeader->pNext) {
			for (ITEM10* pItem = (ITEM10*)pHeader->pItems; pItem; pItem = pItem->pNext) {
				strData += string(pItem->pID) + ' ' + to_string(pItem->Code) + ' ' + to_string(pItem->Date.Day) + ' ' +
					string(pItem->Date.pMonth) + ' ' + to_string(pItem->Date.Year) + '\n';
			}
		}
	}
	char* serData = new char[strData.length() + 2]; // newline and 0
	strcpy_s(serData, strData.length() + 1, strData.c_str()); // normal string giving  problems, c string solved it
	file.write((char*)serData, strlen(serData) * sizeof(char));
	cout << "The requested file was opened and edited successfully!" << endl << endl;
	file.close();
}

// 11 - read datastructure from binary file
DataStructure::DataStructure(char* pFilename) {
	fstream file;
	file.open(pFilename, fstream::in | fstream::binary);
	try {
		if (!file.good()) {
			throw "Error in opening the requested file.";
		}
	}
	catch (const char* e) {
		cout << "ERROR - " << e << endl;
		return;
	}
	string line;
	while (getline(file, line)) { // take input from file and put in string
		ITEM10* pItem = new ITEM10;
		if (line.find(' ', 0) == string::npos) {
			return;
		}
		int pos1 = line.find(' ', 0), pos2 = line.find(' ', pos1 + 1), length = 0;
		pItem->pID = new char[length = line.substr(0, pos2).length() + 1];
		strcpy_s(pItem->pID, length, line.substr(0, pos2).c_str());
		pos1 = line.find(' ', pos2 + 1);
		pItem->Code = stoul(line.substr(pos2 + 1, pos1 - pos2), 0, 0);
		pos2 = line.find(' ', pos1 + 1);
		pItem->Date.Day = stoi(line.substr(pos1 + 1, pos2 - pos1), 0, 0);
		pos1 = line.find(' ', pos2 + 1);
		pItem->Date.pMonth = new char[length = line.substr(pos2 + 1, pos1 - pos2).length() + 1];
		strcpy_s(pItem->Date.pMonth, length, line.substr(pos2 + 1, pos1 - pos2).c_str());
		pos2 = line.find('\n', pos1 + 1); // last character is newline
		pItem->Date.Year = stoi(line.substr(pos1 + 1, pos2 - pos1), 0, 0);
		pItem->pNext = 0;
		*this += pItem;
	}
}

// 12 - assign a datastructure to another
DataStructure& DataStructure::operator=(const DataStructure& Right) {
	if (Right.ppStruct == ppStruct) {
		return *this;
	}
	for (int i = 0; i < 26; i++) { // clear the datatstructure
		for (HEADER_A* pHeader = *(ppStruct + i); pHeader; ) {
			for (ITEM10* pItem = (ITEM10*)pHeader->pItems; pItem; pItem = pItem->pNext) {
				if (!pItem->pNext) {
					delete pItem;
					break;
				}
				if (pItem) {
					delete pItem;
				}
			}
			if (!pHeader->pNext) {
				delete pHeader;
				break;
			}
			if (pHeader) {
				HEADER_A* temp = pHeader->pNext;
				delete pHeader;
				pHeader = temp;
			}
		}
		*(ppStruct + i) = 0;
	}
	for (int i = 0; i < 26; i++) {
		for (HEADER_A* pHeader = *(Right.ppStruct + i); pHeader; pHeader = pHeader->pNext) {
			ITEM10* pItemCopy = new ITEM10;
			for (ITEM10* pItem = (ITEM10*)pHeader->pItems; pItem; pItem = pItem->pNext) {
				pItemCopy->Code = pItem->Code;
				pItemCopy->Date = pItem->Date;
				pItemCopy->pID = pItem->pID;
				pItemCopy->pNext = 0;
				*this += pItemCopy;
			}
		}
	}
	return *this;
}