#ifndef _GRAM_DICT_ORDER
#define _GRAM_DICT_ORDER

#include "StrComp.h"

#include "HashFunction.h"

//#define BUCKET 8
//#define MAXBIT 7

class GramDictOrder : public StrComparator
{
public:
	GramDictOrder(int q, int length, int bSize, int mBit);
	virtual ~GramDictOrder();

	virtual bool isBefore(char* str1, char* str2);
	virtual bool isEqual(char* str1, char* str2);
	virtual bool isNotAfter(char* str1, char* str2);

	virtual float LowerBound(char* qString, int qStrLen, char* sString, 
		char* eString, float threshold);
	virtual float LowerBound(char* lsString, char* leString, char* rsString, char* reString, float threshold);
	virtual int UpperBound(char* qString, char* sString, char* eString);

	virtual int PreProcessing(char* OrgString, char* newString);
	virtual int OrgProcessing(char* OrgString, char* newString);

	virtual char* PrintString(char* pString);

	int GetTrueSize(char* mystr);

	char* RemoveHead(char* str);

	virtual void CompressString(char* mystr);

	virtual void PrintHead(ofstream& output, char* mystr);

	virtual void QueryInfo(char* queryString, int qStrLen);
private:

protected:

	int bucketSize;

	int bucketVol;

	int gramSize;
	int maxBit;

	int* sHashBucket;
	int* eHashBucket;
	int* qHashBucket;

	int* lbArray;
	int* ubArray;

	char* fixedQString;

	void CalculateBucket(char* str, int stringlen, int* buckets);

	int GetStringHash(int startpos, int stringlen, int base);

	char* RemoveHeadAndGram(char* str, int* buckets);

	char* RemoveEmptyHead(char* str);

	int* rHashBucket;
	int* lbNewArray;
	int* ubNewArray;

};

#endif

