#ifndef _STR_DICT_ORDER_H_
#define _STR_DICT_ORDER_H_

#include "StrComp.h"

class StrDictOrder : public StrComparator 
{
public:
	StrDictOrder(int length);
	virtual ~StrDictOrder();

	virtual bool isBefore(char* str1, char* str2);
	virtual bool isEqual(char* str1, char* str2);
	virtual bool isNotAfter(char* st1, char* str2);

	virtual float LowerBound(char* qString, int qStrLen, char* sString, 
		char* eString, float threshold);
	virtual float LowerBound(char* lsString, char* leString, char* rsString, char* reString, float threshold);
	virtual int UpperBound(char* qString, char* sString, char* eString);

	//int Dist(char* qString, char* tString, int threshold);

	virtual int PreProcessing(char* OrgString, char* newString);
	virtual int OrgProcessing(char* OrgString, char* newString);

	virtual char* PrintString(char* pString);

	int GetTrueSize(char* mystr){return strlen(mystr) + 1;};
private:

};

#endif


