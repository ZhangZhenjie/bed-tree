#include <cstring>
#include <cmath>
#include <iostream>

#include "GramDictOrder.h"
//#define _DEBUG_

using namespace std;

GramDictOrder::GramDictOrder(int q, int length, int bSize,
							 int mBit) : StrComparator(length)
{
	gramSize = q;

	maxBit=mBit;
	bucketSize = bSize;
	bucketVol = (bucketSize + 1) * sizeof(int);

	sHashBucket = new int[bucketSize];
	eHashBucket = new int[bucketSize];
	qHashBucket = new int[bucketSize];

	lbArray = new int[bucketSize];
	ubArray = new int[bucketSize];

	rHashBucket = new int[bucketSize];
	lbNewArray = new int[bucketSize];
	ubNewArray = new int[bucketSize];


	int i;

	for(i = 0; i < bucketSize; i++)
	{
		*((int*)(maxString + i * sizeof(int))) = (1 << (maxBit + 1) ) - 1;
		*((int*)(minString + i * sizeof(int))) = 1;
	}

	strcpy(maxString + bucketVol, "100 ");
	strcpy(minString + bucketVol, "100 ");

}

GramDictOrder::~GramDictOrder()
{
	delete [] sHashBucket;
	delete [] eHashBucket;
	delete [] qHashBucket;

	delete [] lbArray;
	delete [] ubArray;
}

bool GramDictOrder::isBefore(char* str1, char* str2)
{
	int i;

	str1 = RemoveHeadAndGram(str1, sHashBucket);
	str2 = RemoveHeadAndGram(str2, qHashBucket);

	//int str1Len = strlen(str1);
	//int str2Len = strlen(str2);

	
	int maxVal = 0;

	for(i = 0; i < bucketSize; i++)
	{
		if(sHashBucket[i] > maxVal)
			maxVal = sHashBucket[i];
		if(qHashBucket[i] > maxVal)
			maxVal = qHashBucket[i];
	}
	maxBit = 0;

	while((maxVal >> maxBit) != 0)
		maxBit++;
	maxBit--;
	

	int bitpos = maxBit;

	for(bitpos = maxBit; bitpos >= 0; bitpos--)
	{
		for(i = 0; i < bucketSize; i++)
		{
			if( ( sHashBucket[i] >> bitpos ) < ( qHashBucket[i] >> bitpos ) )
				return true;
			if( ( sHashBucket[i] >> bitpos ) > ( qHashBucket[i] >> bitpos ) )
				return false;
		}
	}

	return false;
}

bool GramDictOrder::isEqual(char* str1, char* str2)
{
	//str1 = RemoveHeadAndGram(str1, sHashBucket);
	//str2 = RemoveHeadAndGram(str2, qHashBucket);

	//int str1Len = strlen(str1);
	//int str2Len = strlen(str2);

	//CalculateBucket(str1, str1Len, sHashBucket);
	//CalculateBucket(str2, str2Len, qHashBucket);

	int i;

	for(i = 0; i < bucketSize * sizeof(int); i++)
	{
		if(str1[i] != str2[i])
			return false;
	}

	return true;
}

bool GramDictOrder::isNotAfter(char* str1, char* str2)
{
	return !isBefore(str2, str1);
}

float GramDictOrder::LowerBound(char* lsString, char* leString, char* rsString, char* reString, float threshold)
{
	int i;

	lbCompTime++;

	lsString = RemoveHeadAndGram(lsString, qHashBucket);
	leString = RemoveHeadAndGram(leString, rHashBucket);
	rsString = RemoveHeadAndGram(rsString, sHashBucket);
	reString = RemoveHeadAndGram(reString, eHashBucket);

	//////////////////////////////////////////////////////////
	int maxVal = 0;

	for(i = 0; i < bucketSize; i++)
	{
		if(sHashBucket[i] > maxVal)
			maxVal = sHashBucket[i];
		if(eHashBucket[i] > maxVal)
			maxVal = eHashBucket[i];
	}
	maxBit = 0;

	while((maxVal >> maxBit) != 0)
		maxBit++;
	maxBit--;
	////////////////////////////////////////////////////////////

	int bitpos;

	for(i = 0; i < bucketSize; i++)
	{
		lbArray[i] = 0;
		ubArray[i] = ( 1 << (maxBit + 1) ) - 1;
	}

	for(bitpos = maxBit; bitpos >= 0; bitpos--)
	{
		for(i = 0; i < bucketSize; i++)
		{
			if( ( sHashBucket[i] >> bitpos ) < ( eHashBucket[i] >> bitpos ) )
			{
				break;
			}

			int bit = (sHashBucket[i] >> bitpos) % 2;

			if(bit == 0)
			{
				ubArray[i] -= ( 1 << bitpos );
			}
			else
			{
				lbArray[i] += ( 1 << bitpos );
			}
		}

		if(i < bucketSize)
			break;
	}
	/////////////////////////////////////////////////////////////////
	maxVal = 0;

	for(i = 0; i < bucketSize; i++)
	{
		if(qHashBucket[i] > maxVal)
			maxVal = qHashBucket[i];
		if(rHashBucket[i] > maxVal)
			maxVal = rHashBucket[i];
	}
	maxBit = 0;

	while((maxVal >> maxBit) != 0)
		maxBit++;
	maxBit--;

	/////////////////////////////////////////////////////////////////
	for(i = 0; i < bucketSize; i++)
	{
		lbNewArray[i] = 0;
		ubNewArray[i] = ( 1 << (maxBit + 1) ) - 1;
	}

	for(bitpos = maxBit; bitpos >= 0; bitpos--)
	{
		for(i = 0; i < bucketSize; i++)
		{
			if( ( qHashBucket[i] >> bitpos ) < ( rHashBucket[i] >> bitpos ) )
			{
				break;
			}

			int bit = (qHashBucket[i] >> bitpos) % 2;

			if(bit == 0)
			{
				ubNewArray[i] -= ( 1 << bitpos );
			}
			else
			{
				lbNewArray[i] += ( 1 << bitpos );
			}
		}

		if(i < bucketSize)
			break;
	}
	////////////////////////////////////////////////////////////////////

	int sur_sum = 0;
	int exc_sum = 0;

	int minstrlen = 0;
	int maxstrlen = 0;
	int newminstrlen = 0;
	int newmaxstrlen = 0;

	int newThreshold = (int)(threshold * gramSize) + 1;

	for(i = 0; i < bucketSize; i++)
	{
		minstrlen += lbArray[i];
		maxstrlen += ubArray[i];

		if(ubNewArray[i] < lbArray[i])
		{
			sur_sum += ( lbArray[i] - ubNewArray[i] );
			if(sur_sum > newThreshold)
				return threshold + 1;
			continue;
		}

		newminstrlen += lbNewArray[i];
		newmaxstrlen += ubNewArray[i];

		if(lbNewArray[i] > ubArray[i])
		{
			exc_sum += ( lbNewArray[i] - ubArray[i] );
			if(exc_sum > newThreshold)
				return threshold + 1;
			continue;
		}
	}

	//minstrlen -= gramSize - 1;
	//minstrlen = minstrlen > qStrLen ? minstrlen : qStrLen;

	//int retval = (sur_sum + exc_sum + 2 * gramSize - 1) / gramSize / 2;

	int retval;

	
	if(sur_sum > exc_sum)
		retval = (sur_sum + gramSize - 1) / gramSize;
	else
		retval = (exc_sum + gramSize - 1) / gramSize;

	if(newmaxstrlen < minstrlen)
	{
		if(minstrlen - gramSize + 1 - newmaxstrlen > retval)
			retval = minstrlen - newmaxstrlen - gramSize + 1;
	}
	
	if(newminstrlen > maxstrlen)
	{
		if(newminstrlen - gramSize + 1 - maxstrlen > retval)
			retval = newminstrlen - minstrlen - gramSize + 1;
	}

	return retval;
}

float GramDictOrder::LowerBound(char* qString, int qStrLen, char* sString, 
							  char* eString, float threshold)
{
	int i;

	lbCompTime++;

	qString = fixedQString;
	//qString = RemoveHeadAndGram(qString, qHashBucket);
	sString = RemoveHeadAndGram(sString, sHashBucket);
	eString = RemoveHeadAndGram(eString, eHashBucket);

	//int qStrLen = strlen(qString);

	////////////////////////////////////////////////////////////
	int maxVal = 0;

	for(i = 0; i < bucketSize; i++)
	{
		if(sHashBucket[i] > maxVal)
			maxVal = sHashBucket[i];
		if(eHashBucket[i] > maxVal)
			maxVal = eHashBucket[i];
	}
	maxBit = 0;

	while((maxVal >> maxBit) != 0)
		maxBit++;
	maxBit--;
	////////////////////////////////////////////////////////////

	int bitpos;


	for(i = 0; i < bucketSize; i++)
	{
		lbArray[i] = 0;
		ubArray[i] = ( 1 << (maxBit + 1) ) - 1;
	}

	for(bitpos = maxBit; bitpos >= 0; bitpos--)
	{
		for(i = 0; i < bucketSize; i++)
		{
			if( ( sHashBucket[i] >> bitpos ) < ( eHashBucket[i] >> bitpos ) )
			{
				break;
			}

			int bit = (sHashBucket[i] >> bitpos) % 2;

			if(bit == 0)
			{
				ubArray[i] -= ( 1 << bitpos );
			}
			else
			{
				lbArray[i] += ( 1 << bitpos );
			}
		}

		if(i < bucketSize)
			break;
	}

	int sur_sum = 0;
	int exc_sum = 0;

	int minstrlen = 0;
	int maxstrlen = 0;

	int newThreshold = (int)(threshold * gramSize) + 1;

	for(i = 0; i < bucketSize; i++)
	{
		minstrlen += lbArray[i];
		maxstrlen += ubArray[i];

		if(qHashBucket[i] >= lbArray[i] && qHashBucket[i] <= ubArray[i])
			continue;

		if(qHashBucket[i] < lbArray[i])
		{
			sur_sum += ( lbArray[i] - qHashBucket[i] );
			if(sur_sum > newThreshold)
				return threshold + 1;
			continue;
		}

		exc_sum += ( qHashBucket[i] - ubArray[i]);
		if(exc_sum > newThreshold)
			return threshold + 1;
	}

	//minstrlen -= gramSize - 1;
	//minstrlen = minstrlen > qStrLen ? minstrlen : qStrLen;

	//int retval = (sur_sum + exc_sum + 2 * gramSize - 1) / gramSize / 2;

	int retval;

	
	if(sur_sum > exc_sum)
		retval = (sur_sum + gramSize - 1) / gramSize;
	else
		retval = (exc_sum + gramSize - 1) / gramSize;
	

	if(qStrLen < minstrlen)
	{
		if(minstrlen - gramSize + 1 - qStrLen > retval)
			retval = minstrlen - qStrLen - gramSize + 1;
	}
	
	if(qStrLen > maxstrlen)
	{
		if(qStrLen - gramSize + 1 - maxstrlen > retval)
			retval = qStrLen - minstrlen - gramSize + 1;
	}

	return retval;
}

int GramDictOrder::UpperBound(char* qString, char* sString, char* eString)
{
	return strlen(qString);
}


void GramDictOrder::CalculateBucket(char* string, int stringlen, int* buckets)
{
	int i;

	//int* valarray = matrix[0];
	//for(i = 0; i < stringlen; i++)
	//	valarray[i] = HashFunction::GetUniHashValue(string[i], bucketSize);

	//for(i = 1; i < stringlen; i++)
	//	valarray[i] += valarray[i - 1];

	for(i = 0; i < bucketSize; i++)
		buckets[i] = 0;

	for(i = 1 - gramSize; i < stringlen; i++)
	{
		int startpos;
		int tempstrlen;

		if(i < 0)
		{
			startpos = 0;
			tempstrlen = gramSize + i;
			if(tempstrlen > stringlen)
				tempstrlen = stringlen;
		}
		else
		{
			startpos = i;
			tempstrlen = gramSize;

			if(i + gramSize > stringlen)
				tempstrlen = stringlen - i;
		}

		if(tempstrlen > 0)
		{
			int bucketID = HashFunction::GetStringHashValue(string + startpos, tempstrlen, bucketSize);

			buckets[bucketID]++;
		}
	}

#ifdef _DEBUG_
	for(i = 0; i < bucketSize; i++)
		cout << buckets[i] << " ";
	cout << endl;
#endif
}


/*
void GramDictOrder::CalculateBucket(char* str, int stringlen, int* buckets)
{
	int i;

	for(i = 0; i < stringlen; i++)
		buckets[i] = str[i] - 1;
}
*/


int GramDictOrder::GetStringHash(int startpos, int stringlen, int base)
{
	int* valarray = matrix[0];

	/*
	int sum = 0;

	for(i = 0; i < strlen; i++)
	{
		sum += valarray[startpos + i];
	}
	*/

	return (valarray[startpos + stringlen - 1] - valarray[startpos] )  % base;
}

int GramDictOrder::PreProcessing(char* OrgString, char* newString)
{
	char* newOrgString = RemoveHead(OrgString);

	int newOrgLen = strlen(newOrgString);
	*((int*)(newString + bucketSize * sizeof(int))) = strlen(OrgString + bucketVol) + 1;

	int i;

	for(i = 0; i < newOrgLen; i++)
	{
		if(newOrgString[i] >= 'A' && newOrgString[i] <= 'Z')
			newOrgString[i] -= 'A' - 'a';
	}

	CalculateBucket(newOrgString, newOrgLen, qHashBucket);


	for(i = 0; i < bucketSize; i++)
	{
		if(qHashBucket[i] < 0)
		{
			cout << "error" << endl;
		}
		*((int*)(newString + i * sizeof(int))) = qHashBucket[i] + 1;

		//cout << qHashBucket[i] << " ";

		//if(newString[i] == '\0')
		//	cout << "wrong" << endl;
	}
	//cout << endl;

	strcpy(newString + bucketVol, OrgString);

	return newOrgLen;
	/*
	int orgstrlen = strlen(OrgString);

	for(i = 0; i < orgstrlen; i++)
	{
		if(newString[bucketVol + i] >= 'A' && newString[bucketSize + i] <= 'Z')
			newString[bucketVol + i] -= 'Z' - 'A';
	}
	*/
}

int GramDictOrder::OrgProcessing(char* OrgString, char* newString)
{
	char* newOrgString = RemoveEmptyHead(OrgString);

	int newOrgLen = strlen(newOrgString);

	*((int*)(newString + bucketSize * sizeof(int))) = strlen(OrgString) + 1;

	int i;

	for(i = 0; i < newOrgLen; i++)
	{
		if(newOrgString[i] >= 'A' && newOrgString[i] <= 'Z')
			newOrgString[i] -= 'A' - 'a';
	}

	CalculateBucket(newOrgString, newOrgLen, qHashBucket);


	for(i = 0; i < bucketSize; i++)
	{
		if(qHashBucket[i] < 0)
		{
			cout << "error" << endl;
		}
		*((int*)(newString + i * sizeof(int))) = qHashBucket[i] + 1;

		//cout << qHashBucket[i] << " ";

		//if(newString[i] == '\0')
		//	cout << "wrong" << endl;
	}
	//*((int*)(newString + i * sizeof(int))) = newOrgLen + 1;

	//cout << endl;

	strcpy(newString + bucketVol, OrgString);

	return newOrgLen;
	/*
	int orgstrlen = strlen(OrgString);

	for(i = 0; i < orgstrlen; i++)
	{
		if(newString[bucketVol + i] >= 'A' && newString[bucketSize + i] <= 'Z')
			newString[bucketVol + i] -= 'Z' - 'A';
	}
	*/
}

char* GramDictOrder::PrintString(char* pString)
{
	char* tmpstr = pString + bucketVol;

	while(*tmpstr != ' ')
		tmpstr++;

	while(*tmpstr == ' ')
		tmpstr++;

	return tmpstr;
}

char* GramDictOrder::RemoveHeadAndGram(char* str, int* buckets)
{
	int i;

	for(i = 0; i < bucketSize; i++)
		buckets[i] = *((int*)(str + i * sizeof(int))) - 1;

	char* tempstr = str + bucketVol;

	if( *tempstr == '\0')
		return tempstr;

	while( *tempstr != ' ')
		tempstr++;

	while( *tempstr == ' ')
		tempstr++;

	return tempstr;
	
}

int GramDictOrder::GetTrueSize(char* mystr)
{
	int mystrlen = *((int*)(mystr +  bucketSize * sizeof(int)));

	return bucketVol + mystrlen;

	//return sizeof(int) * bucketSize + 1;
}

char* GramDictOrder::RemoveHead(char* str)
{
	str += bucketVol;

	if(*str == '\0')
		return str;

	while( *str != ' ')
		str++;

	while( *str == ' ')
		str++;

	return str;
}

char* GramDictOrder::RemoveEmptyHead(char* mystr)
{
	if( *mystr == '\0')
		return mystr;

	while( *mystr != ' ')
		mystr++;

	while( *mystr == ' ')
		mystr++;

	return mystr;
}

void GramDictOrder::CompressString(char* mystr)
{
	mystr += bucketVol;

	///*
	if(*mystr == '\0')
		return;

	while( *mystr != ' ')
		mystr++;

	while( *mystr == ' ')
		mystr++;
	//*/

	*mystr = '\0';

	return;
}

void GramDictOrder::PrintHead(ofstream& output, char* mystr)
{
	int i;

	for(i = 0; i < bucketSize; i++)
		output << *((int*)(mystr + i * sizeof(int))) << " ";
	output << endl;
}

void GramDictOrder::QueryInfo(char* qString, int qStrLen)
{
	int i;

	for(i = 0; i < bucketSize; i++)
	{
		qHashBucket[i] = *((int*)(qString + i * sizeof(int))) - 1;
	}

	qString = RemoveHead(qString);

	fixedQString = qString;

	for(i = 0; i < 26; i++)
		qLetterCounter[i] = 0;

	for(i = 0; i < qStrLen; i++){
		int pos = qString[i] - 'a';

		if(pos >= 0 && pos < 26)
			qLetterCounter[pos]++;
	}

}
