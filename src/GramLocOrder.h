#ifndef _GRAM_LOC_ORDER_
#define _GRAM_LOC_ORDER_

#include "GramDictOrder.h"
#include "InvertedList.h"

class GramLocOrder : public GramDictOrder
{
public:

	GramLocOrder(int q, int length, int bSize, int mBit, InvertedList* l);
	virtual ~GramLocOrder();

	virtual bool isBefore(char* str1, char* str2);
	virtual bool isEqual(char* str1, char* str2);
	virtual bool isNotAfter(char* str1, char* str2);

	virtual float LowerBound(char* qString, int qStrLen, char* sString, 
		char* eString, float threshold);
	virtual int UpperBound(char* qString, char* sString, char* eString);

	virtual int PreProcessing(char* OrgString, char* newString);
	virtual int OrgProcessing(char* OrgString, char* newString);

	virtual char* PrintString(char* pString);

	//virtual void CompressString(char* mystr);
private:

	InvertedList* list;
	int gramNum;

	bool CompareGram(int hval, int loc, int preHval, int preLoc,
		int* lbVal, int* ubVal, int* lbLoc, int* ubLoc, int bitpos);

	bool CompareGram(int hval, int loc, int preHval, int preLoc);
};

#endif


