#ifndef _STR_COMPARATOR_H_
#define _STR_COMPARATOR_H_

#include <cstring>
#include <fstream>

using namespace std;

class StrComparator
{
public:
	StrComparator(int length);
	virtual ~StrComparator();

	virtual bool isBefore(char* str1, char* str2);
	virtual bool isEqual(char* str1, char* str2);
	virtual bool isNotAfter(char* st1, char* str2);

	//what's the minimum distance from query string, if we know the 
	//beginning and ending string in the interval
	virtual float LowerBound(char* qString, int qStrLen, char* sString, 
		char* eString, float threshold);

	//pairwise lower bound between two string intervals
	virtual float LowerBound(char* lsString, char* leString, char* rsString, char* reString, float threshold);

	//upper bound instead of lower bound
	virtual int UpperBound(char* qString, char* sString, char* eString);

	virtual float Dist(char* qString, char* tString, float threshold);
	virtual float Dist(char* qString, int qStrLen, char* tString, float threshold);

	int GetDistCompTime(){return distCompTime;};
	int GetLBCompTime(){return lbCompTime;};

	virtual int PreProcessing(char* orgString, char* newString);
	virtual int OrgProcessing(char* orgString, char* newString);

	virtual char* PrintString(char* pString);

	char* GetMinString(){return minString;};
	char* GetMaxString(){return maxString;};
	virtual char* RemoveHead(char* str);

	virtual int GetTrueSize(char* mystr){return strlen(mystr) + 1;};

	virtual void CompressString(char* mystr);

	virtual void PrintHead(ofstream& output, char* mystr);

	virtual void QueryInfo(char* queryString, int qStrLen);
private:

protected:
	int maxSize;
	int** matrix;

	bool isSameChar(char* sString, int sStrLen, char* eString, int eStrLen, 
		int index);

	bool isBetween(char* sString, char* eString, int pos, char* qString, int qPos);


	int distCompTime;
	int lbCompTime;

	char minString[2000];
	char maxString[2000];

	int  tLetterCounter[26];
	int  qLetterCounter[26];
};

#endif

