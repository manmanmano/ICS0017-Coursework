#include <iostream>
#include <string.h>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"

// Mariano D'Angelo --> Struct 3 = HEADER_A **GetStruct3(int iItem, int nItems), Item10 & DATE3

char FindSecondChar(char* pNewItemID);
void PrintDataStructure(HEADER_A** ppStruct);
HEADER_A** InsertItem(HEADER_A** ppStruct, char* pNewItemID = 0);
HEADER_A** RemoveItem(HEADER_A** ppStruct, char* pItemID);

int main() {
	HEADER_A** pps3 = GetStruct3(10, 30); // pps3 - double pointer to Structure 3
	printf("The data structure contains:\n\n");
	PrintDataStructure(pps3);
	char items[15][5] = { "Z A", "Z Z", "Z K", "A Z", "A A", "A K", "G Z", "G A", "G K", "M A", "M Ba", "M Bb", "M Z", "M Ba", "Mba" };
	printf("\n\n\nThe data structure after insertion:\n\n");
	for (int i = 0; i < 15; i++) {
		pps3 = InsertItem(pps3, items[i]);
	}
	PrintDataStructure(pps3);
	printf("\n\n\nThe data structure after deletion:\n\n");
	for (int i = 0; i < 15; i++) {
		pps3 = RemoveItem(pps3, items[i]);
	}
	PrintDataStructure(pps3);
	return 0;
}

char FindSecondChar(char* pNewItemID) {
	char* pSecondId = (char*)' ', secondChar;
	char* pSpace = strchr(pNewItemID, ' '); // find the second word of ID
	pSecondId = pSpace + 1;
	secondChar = *(pSecondId);
	return secondChar;
}

void PrintDataStructure(HEADER_A** ppStruct) {
	int j = 1;
	if (!ppStruct) {
		printf("The chosen data structure is emtpy!\n");
	}
	else {
		for (int i = 0; i < 26; i++) {
			for (HEADER_A* pHeader = *(ppStruct + i); pHeader; pHeader = pHeader->pNext) {
				for (ITEM10* pItem = (ITEM10*)pHeader->pItems; pItem; pItem = pItem->pNext, j++) {
					printf("%d) %s %lu %d %s %d\n", j, pItem->pID, pItem->Code, pItem->Date.Day, pItem->Date.pMonth, pItem->Date.Year);
				}
			}
		}
	}
}

HEADER_A** InsertItem(HEADER_A** ppStruct, char* pNewItemID) {
	ITEM10* pNewItem;
	if (!ppStruct) { // if the data structure is empty return the empty data structure
		printf("The chosen data structure is emtpy!\n");
		return ppStruct;
	}
	char* afterSpace = strchr(pNewItemID, ' ');
	if (pNewItemID) {
		if (strlen(pNewItemID) < 3 || !strstr(pNewItemID, " ") || !strpbrk(afterSpace + 1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ")) {
			printf("- Item with ID '%s' does not follow the formatting rules, not inserted.\n\n", pNewItemID);
			return ppStruct;
		}
		pNewItem = (ITEM10*)GetItem(10, pNewItemID);
	}
	else {
		pNewItem = (ITEM10*)GetItem(10);
	}
	char firstChar = *(pNewItem->pID), secondChar = FindSecondChar(pNewItem->pID);
	if (firstChar < 'A' || firstChar > 'Z' || secondChar < 'A' || secondChar > 'Z') {
		printf("- Item with ID '%s' does not follow the formatting rules, not inserted.\n\n", pNewItem->pID);
		return ppStruct;
	}
	for (HEADER_A* pHeader = *((ppStruct + firstChar) - 65); pHeader; pHeader = pHeader->pNext) {
		for (ITEM10* pItem = (ITEM10*)pHeader->pItems; pItem; pItem = pItem->pNext) {
			if (!strcmp(pItem->pID, pNewItem->pID)) {
				printf("- Item with ID '%s' already exists, not inserted.\n\n", pNewItem->pID);
				return ppStruct;
			}
		}
	}
	HEADER_A* pHeader = *((ppStruct + firstChar) - 65), * pNewHeader;
	if (!*((ppStruct + firstChar) - 65)) {
		pNewHeader = (HEADER_A*)malloc(sizeof(HEADER_A));
		pNewHeader->cBegin = secondChar;
		pNewHeader->pItems = pNewItem;
		*((ppStruct + firstChar) - 65) = pNewHeader;
		pNewHeader->pNext = 0;
		return ppStruct;
	}
	else {
		for (; pHeader; pHeader = pHeader->pNext) {
			if (secondChar == pHeader->cBegin) {
				pNewItem->pNext = (ITEM10*)pHeader->pItems;
				pHeader->pItems = pNewItem;
				break;
			}
			else if (secondChar < pHeader->cBegin) {
				pNewHeader = (HEADER_A*)malloc(sizeof(HEADER_A));
				pNewHeader->cBegin = secondChar;
				pNewHeader->pItems = pNewItem;
				*((ppStruct + firstChar) - 65) = pNewHeader;
				pNewHeader->pNext = pHeader;
				break;
			}
			else {
				if (!pHeader->pNext) {
					pNewHeader = (HEADER_A*)malloc(sizeof(HEADER_A));
					pNewHeader->cBegin = secondChar;
					pNewHeader->pItems = pNewItem;
					pHeader->pNext = pNewHeader;
					pNewHeader->pNext = 0;
					break;
				}
				else if (secondChar > pHeader->cBegin && secondChar < pHeader->pNext->cBegin) {
					pNewHeader = (HEADER_A*)malloc(sizeof(HEADER_A));
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
		return ppStruct;
	}
}

HEADER_A** RemoveItem(HEADER_A** ppStruct, char* pItemID) {
	if (!ppStruct) { // if the data structure is empty return the empty data structure
		printf("The chosen data structure is emtpy!\n");
		return ppStruct;
	}
	char* afterSpace = strchr(pItemID, ' ');
	if (strlen(pItemID) < 3 || !strstr(pItemID, " ") || !strpbrk(afterSpace + 1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ")) {
		printf("- Item with ID '%s' does not follow the formatting rules, not deleted.\n\n", pItemID);
		return ppStruct;
	}
	char firstChar = *(pItemID), secondChar = FindSecondChar(pItemID);
	if (firstChar < 'A' || firstChar > 'Z' || secondChar < 'A' || secondChar > 'Z') {
		printf("- Item with ID '%s' does not follow the formatting rules, not deleted.\n\n", pItemID);
		return ppStruct;
	}
	for (HEADER_A* pHeader = *((ppStruct + firstChar) - 65), *pPrevHeader = 0; pHeader; pPrevHeader = pHeader, pHeader = pHeader->pNext) {
		if (secondChar == pHeader->cBegin) {
			for (ITEM10* pItem = (ITEM10*)pHeader->pItems, *pPrevItem = 0; pItem; pPrevItem = pItem, pItem = pItem->pNext) {
				if (!strcmp(pItemID, pItem->pID)) {
					if (!pPrevItem && !pItem->pNext) {
						if (pHeader == *((ppStruct + firstChar) - 65) && pHeader->pNext) {
							*((ppStruct + firstChar) - 65) = pHeader->pNext;
							free(pItem);
							free(pHeader);
							return ppStruct;
						}
						else if (pHeader == *((ppStruct + firstChar) - 65) && !pHeader->pNext) {
							*((ppStruct + firstChar) - 65) = 0;
							free(pItem);
							free(pHeader);
							return ppStruct;
						}
						else if (pHeader != *((ppStruct + firstChar) - 65) && !pHeader->pNext) {
							pPrevHeader->pNext = 0;
							free(pItem);
							free(pHeader);
							return ppStruct;
						}
						else {
							pPrevHeader->pNext = pHeader->pNext;
							free(pItem);
							free(pHeader);
							return ppStruct;
						}
					}
					else if (pPrevItem && !pItem->pNext) {
						pPrevItem->pNext = 0;
						free(pItem);
						return ppStruct;
					}
					else if (!pPrevItem && pItem->pNext) {
						pHeader->pItems = pItem->pNext;
						free(pItem);
						return ppStruct;
					}
					else {
						pPrevItem->pNext = pItem->pNext;
						free(pItem);
						return ppStruct;
					}
				}
			}
		}
	}
	printf("- Item with ID '%s' does not exist, not deleted.\n\n", pItemID);
	return ppStruct;
}