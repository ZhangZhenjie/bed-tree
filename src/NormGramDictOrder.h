#ifndef _NORM_GRAM_DICT_ORDER_
#define _NORM_GRAM_DICT_ORDER_

#include "GramDictOrder.h"

class NormGramDictOrder : public GramDictOrder
{
public:
	NormGramDictOrder(int q, int length, int bSize, int mBit);
	virtual ~NormGramDictOrder();

	virtual float Dist(char* qString, char* tString, float threshold);
	
	virtual float Dist(char* qString, int qStrLen, char* tString, float threshold);

	virtual float LowerBound(char* qString, int qStrLen, char* sString, 
		char* eString, float threshold);
	
	virtual float LowerBound(char* lsString, char* leString, char* rsString, char* reString, float threshold);
};

#endif


