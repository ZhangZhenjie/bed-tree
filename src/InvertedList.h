#ifndef _INVERTED_LIST_H_
#define _INVERTED_LIST_H_

#include "Page.h"
#include "BufferManager.h"
#include "StrComp.h"

#include <fstream>

using namespace std;

class InvertedList : public Page
{
public:
	InvertedList(int bSize, int gSize, int length, StrComparator* comp, 
		BufferManager* manager);
	InvertedList(int bSize, int gSize, int length, StrComparator* comp,
		BufferManager* manager, char* mystring, InvertedList* lSibling,
		InvertedList* rSibling,	InvertedList* mychild);
	~InvertedList();

	int RangeQuery(char* queryString, int threshold);

	void ConstructIndex(char* filename, int lineNum, int threshold);
	void ConstructCount(char* filename, int lineNum);

	long GetTotalCount(){return totalCount;};
	InvertedList* SearchGram(char* gramStr);

private:
	//void GetDF(char* filename, int lineNum);
	
	int maxLength;
	int gramSize;

	StrComparator* strcomp;

	ifstream input;

	//long* hashArray;
	int hashArraySize;

	InvertedList** listHashArray;
	long* strPosArray;

	//the followings are used in special cases
	char* hashString;
	InvertedList* leftSibling;
	InvertedList* rightSibling;
	InvertedList* child;
	int longSize;

	int curSize;
	int maxSize;
	long totalSize;
	long totalCount;

	bool isFull(){return (curSize == maxSize);};
	void InsertID(long id);
	void CountID(long id);


	void PrintGram(char* gramStr);

	int PrintIDs();

	int PrintAllIDs();

	void ParseString(char* mystring, int mystrlen, 
		char** gramArray, int* locArray, long* weightArray);

	int FindMinPrefix(char** gramArray, int* locArray, long* weightArray, int testNum,
		int threshold);
};

#endif

