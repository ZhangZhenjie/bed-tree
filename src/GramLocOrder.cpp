#include "GramLocOrder.h"
#include "Queue.h"

#include <cstdlib>
#include <cstring>
#include <iostream>

#define MAGICNUM 1
#undef _DEBUG_

using namespace std;

//note that we use 2 * bSize to store the qgram and location information
GramLocOrder::GramLocOrder(int q, int length, int gNum, int mBit, 
						   InvertedList* l) : GramDictOrder(q, length, 2 * gNum, mBit)
{
	int i;

	gramNum = gNum;

#ifdef _DEBUG_
	ifstream logfile("log.txt");
	logfile.close();
#endif

	for(i = 0; i < bucketSize; i++)
	{
		if(i % 2 == 0)
		{
			*((int*)(maxString+ i * sizeof(int))) = MAGICNUM;
			*((int*)(minString+ i * sizeof(int))) = (1 << (maxBit+1) ) - 1;
		}
		else
		{
			*((int*)(maxString+ i * sizeof(int))) = 1;
			*((int*)(minString+ i * sizeof(int))) = (1 << (maxBit+1)) - 1;
		}
	}

	strcpy(maxString + bucketVol, "100 ");
	strcpy(minString + bucketVol, "100 ");

	
	list = l;
}

GramLocOrder::~GramLocOrder()
{
}

bool GramLocOrder::isBefore(char* str1, char* str2)
{
	int i;

	str1 = RemoveHeadAndGram(str1, sHashBucket);
	str2 = RemoveHeadAndGram(str2, qHashBucket);

	//int str1Len = strlen(str1);
	//int str2Len = strlen(str2);

	/*
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
	*/

	/*
	for(i = 0; i < bucketSize; i++)
	{
		if(sHashBucket[i] < qHashBucket[i])
			return true;
		
		if(sHashBucket[i] > qHashBucket[i])
			return false;
	}
	*/

	for(i = 0; i < gramNum; i++)
	{
		int shval = sHashBucket[2 * i];
		int sloc = sHashBucket[2 * i + 1];

		int qhval = qHashBucket[2 * i];
		int qloc = qHashBucket[2 * i + 1];

		if(shval == qhval && sloc == qloc)
			continue;

		return CompareGram(shval, sloc, qhval, qloc);
	}

	return false;
}

bool GramLocOrder::isEqual(char* str1, char* str2)
{
	str1 = RemoveHeadAndGram(str1, sHashBucket);
	str2 = RemoveHeadAndGram(str2, qHashBucket);

	//int str1Len = strlen(str1);
	//int str2Len = strlen(str2);

	int i;

	for(i = 0; i < bucketSize; i++)
	{
		if(sHashBucket[i] != qHashBucket[i])
			return false;
	}

	return true;
}

bool GramLocOrder::isNotAfter(char* str1, char* str2)
{
	return !isBefore(str2, str1);
}

float GramLocOrder::LowerBound(char* qString, int qStrLen, char* sString, 
							 char* eString, float threshold)
{
	int i,j;

	lbCompTime++;

	qString = fixedQString;
	//qString = RemoveHeadAndGram(qString, qHashBucket);
	sString = RemoveHeadAndGram(sString, sHashBucket);
	eString = RemoveHeadAndGram(eString, eHashBucket);

	//int qStrLen = strlen(qString);

#ifdef _DEBUG_
	ofstream output("log.txt", ios::app);

	for(i = 0; i < gramNum; i++)
	{
		output << qHashBucket[2 * i] << "-" << qHashBucket[2*i + 1] << "\t";
	}
	output << endl;
	
	for(i = 0; i < gramNum; i++)
	{
		output << sHashBucket[2 * i] << "-" << sHashBucket[2*i + 1] << "\t";
	}
	output << endl;
	//if(sHashBucket[0] == 30 && sHashBucket[1] == 0)
	//	cout << endl;
	for(i = 0; i < gramNum; i++)
	{
		output << eHashBucket[2 * i] << "-" << eHashBucket[2*i + 1] << "\t";
	} 
	output << endl;
#endif

	for(i = 0; i < bucketSize; i++)
	{
		lbArray[i] = 0;
		ubArray[i] = (1 << (maxBit + 1)) - 1;
	}

	for(i = 0; i < gramNum; i++)
	{
		if(sHashBucket[2 * i] == eHashBucket[2 * i] &&
			sHashBucket[2 * i + 1] == eHashBucket[2 * i + 1])
		{
			lbArray[2 * i] = ubArray[2 * i] = sHashBucket[2 * i];
			lbArray[2 * i + 1] = ubArray[2 * i + 1] = sHashBucket[2 * i + 1];
		}
		else
		{
			int bitpos = maxBit;

			while(bitpos >= 0)
			{
				if(CompareGram(sHashBucket[2*i], sHashBucket[2*i+1],
								eHashBucket[2*i], eHashBucket[2*i+1],
								lbArray + 2 * i, ubArray + 2 * i, 
								lbArray + 2 * i + 1, ubArray + 2 * i + 1,
								bitpos))
					break;

				bitpos--;
			}
			
			break;
		}
	}

#ifdef _DEBUG_
	for(i = 0; i < gramNum; i++)
	{
		output << lbArray[2 * i] << "-" << lbArray[2*i + 1] << "\t";
	}
	output << endl;
	for(i = 0; i < gramNum; i++)
	{
		output << ubArray[2 * i] << "-" << ubArray[2*i + 1] << "\t";
	}
	output << endl;
#endif

	for(i = 0; i < gramNum; i++)
	{
		if(lbArray[2 * i + 1] - qStrLen > threshold)
		{
#ifdef _DEBUG_
			output << "length pruning" << endl;
			output << endl;
#endif
			return lbArray[2 * i + 1] - qStrLen;
		}
	}

	/*
	bool flag = true;
	int minHashVal = 0;

	for(i = 0; i < bucketSize; i++)
	{
		if(flag)
		{
			if(sHashBucket[i] < eHashBucket[i])
			{
				flag = false;
			}

			lbArray[i] = sHashBucket[i];
			ubArray[i] = eHashBucket[i];

			if(i % 2 == 1)
				minHashVal = lbArray[i];
		}
		else
		{
			if(i % 2 == 1)
				lbArray[i] = minHashVal;
			else
				lbArray[i] = 0;

			ubArray[i] = ( 1 << (maxBit + 1) ) - 1;
		}
	}
	*/


	int lbsum = 0;

	int nextLoc = (1 << (maxBit+1)) - 1;

	int maxLB = -1;
	int minUB = (1 << (maxBit + 1));
	
	for(i = 0; i < gramNum; i++)
	{
		if(lbArray[2 * i] > maxLB)
			maxLB = lbArray[2 * i];
		if(ubArray[2 * i + 1] < minUB)
			minUB = ubArray[2 * i];
	}


	for(i = 0; i < gramNum; i++)
	{
		if(qHashBucket[2 * i + 1] >= nextLoc) 
			continue;

		if(qHashBucket[2 * i] == MAGICNUM - 1)
			continue;

		if(minUB > 0 && qHashBucket[2 * i] >= maxLB )
			continue;

		for(j = 0; j < gramNum; j++)
		{
			if(qHashBucket[2 * i + 1] >= (lbArray[2 * j + 1] - threshold + 1) 
				&& qHashBucket[2 * i + 1] <= (ubArray[2 * j + 1] + threshold - 1) )
			{
				if(qHashBucket[2 * i] >= (lbArray[2 * j])
					&& qHashBucket[2 * i] <= (ubArray[2 * j]) )
					break;
			}
		}

		if(j == gramNum)
		{
			lbsum++;
			nextLoc = qHashBucket[2 * i + 1] - gramSize + 1;
			if(lbsum > threshold)
			{
#ifdef _DEBUG_
				output << "positive pruning " << lbsum << endl;
				output << endl;
#endif
				return lbsum;
			}
		}
	}

	lbsum = 0;
	nextLoc = (1 << (maxBit+1)) - 1;
	maxLB = - 1;

	for(i = 0; i < gramNum; i++)
	{
		if(qHashBucket[2 * i] > maxLB)
			maxLB = qHashBucket[2 * i];
	}

	for(i = 0; i < gramNum; i++)
	{
		if(ubArray[2 * i + 1] >= nextLoc)
			continue;

		if(lbArray[2 * i] >= maxLB)
			continue;

		for(j = 0; j < gramNum; j++)
		{
			if(lbArray[2 * i] <= qHashBucket[2 * j] &&
				ubArray[2 * i] >= qHashBucket[2 * j] &&
				(ubArray[2 * i + 1] >= (qHashBucket[2 * j + 1] - threshold + 1) ) &&
				(lbArray[2 * i + 1] <= (qHashBucket[2 * j + 1] + threshold - 1) ) 
				)
				break;
		}

		if(j == gramNum)
		{
			lbsum++;
			nextLoc = lbArray[2 * i + 1] - gramSize + 1;
			if(lbsum > threshold)
			{
#ifdef _DEBUG_
				output << "reverse pruning " << lbsum << endl;
				output << endl;
#endif
				return lbsum;
			}

			if(nextLoc <= 0)
				break;
		}

		if(lbArray[2 * i] != ubArray[2 * i])
			break;
	}
	//minstrlen -= gramSize - 1;
	//minstrlen = minstrlen > qStrLen ? minstrlen : qStrLen;

#ifdef _DEBUG_
	output << lbsum << endl;
	output << endl;
#endif


	int retval = lbsum;
	return retval;
}

int GramLocOrder::UpperBound(char* qString, char* sString, char* eString)
{
	return strlen(qString);
}

int GramLocOrder::PreProcessing(char* OrgString, char* newString)
{
	char* newOrgString = RemoveHead(OrgString);

	int newOrgLen = strlen(newOrgString);
	*((int*)(newString + bucketSize * sizeof(int))) = strlen(OrgString + bucketVol) + 1;

	int i,j;
	int location, idval;

	for(i = 0; i < newOrgLen; i++)
	{
		if(newOrgString[i] >= 'A' && newOrgString[i] <= 'Z')
			newOrgString[i] -= 'A' - 'a';
	}

	Queue queue(gramNum + 1);

	int base = 1 << (maxBit + 1);

	int* idArray;
	int* hashArray;

	int arraySize = newOrgLen - gramSize + 1;
	if(arraySize > 0)
	{
		idArray = new int[newOrgLen - gramSize + 1];
		hashArray = new int[newOrgLen - gramSize + 1];
	}

	for(i = 0; i < arraySize; i++)
	{
		idArray[i] = i;

		hashArray[i] = HashFunction::GetStringHashValue(newOrgString + i, gramSize, base);

		/*
		InvertedList* gramNode = list->SearchGram(newOrgString + i);

		if(gramNode)
			hashArray[i] = list->SearchGram(newOrgString+i)->GetTotalCount();
		else
			hashArray[i] = 0;

		//cout << idArray[i] << " " << hashArray[i] << endl;

		if(hashArray[i] >= (1 << maxBit))
			continue;
		*/

		queue.Enqueue(idArray + i, (double)hashArray[i]);

		if(queue.GetCurSize() == (gramNum + 1))
		{
			queue.Dequeue();
			//queue.Print();
		}
	}

	void** resArray = queue.GetCurArray();
	int resSize = queue.GetCurSize();


	for(i = 0; i < gramNum; i++)
	{
		if(i < resSize)
		{
			location = *((int*) (resArray[i]));
			*((int*)(newString + (2 * i + 1) * sizeof(int))) = location + 1;
			idval = hashArray[location];
			*((int*)(newString + (2 * i) * sizeof(int))) = idval + 1;
		}
		else
		{
			//*((int*)(newString + (2 * i + 1) * sizeof(int))) = (1 << (maxBit+1))-1;
			//*((int*)(newString + (2 * i) * sizeof(int))) = (1 << (maxBit+1))-1;
			*((int*)(newString + (2 * i) * sizeof(int))) = 1;
			*((int*)(newString + (2 * i + 1) * sizeof(int))) = 1;
		}

		//cout << location << "-" << idval << " ";
	}
	//cout << endl;

	for(i = 0; i < gramNum - 1; i++)
	{
		for(j = gramNum - 1; j > i; j--)
		{
			int tempLoc = *((int*)(newString + (2 * j + 1) * sizeof(int)));
			int tempHash = *((int*)(newString + (2 * j) * sizeof(int)));
			int tempPreLoc = *((int*)(newString + (2 * j - 1) * sizeof(int)));
			int tempPreHash = *((int*)(newString + (2 * j - 2) * sizeof(int)));

			if(tempLoc > tempPreLoc)
			//if(CompareGram(tempLoc, tempHash, tempPreLoc, tempPreHash))
			{
				*((int*)(newString + 2 * j * sizeof(int))) = 
					*((int*)(newString + (2 * j - 2) * sizeof(int)));
				*((int*)(newString + (2 * j + 1) * sizeof(int))) = 
					*((int*)(newString + (2 * j - 1) * sizeof(int)));
				*((int*)(newString + (2 * j - 2) * sizeof(int))) = 
					tempHash;
				*((int*)(newString + (2 * i - 1) * sizeof(int))) = 
					tempLoc;
			}
		}

		//for(j = 0; j < gramNum; j++)
		//	cout << (int)newString[2*j] << "-" << (int)newString[2*j+1] << " ";
		//cout << endl;
	}

		
	for(i = 0; i < gramNum; i++)
	{
		int location = *((int*)(newString + (2 * i + 1) * sizeof(int)));
		int hashval  = *((int*)(newString + (2 * i) * sizeof(int)));

		if(location < 0 || hashval < 0)
			cout << location << "-" << hashval << " ";
	}
	//cout << endl;
	

	strcpy(newString + bucketVol, OrgString);

	if(arraySize > 0)
	{
		delete [] idArray;
		delete [] hashArray;
	}

	return newOrgLen;
}

int GramLocOrder::OrgProcessing(char* OrgString, char* newString)
{
	char* newOrgString = RemoveEmptyHead(OrgString);

	int newOrgLen = strlen(newOrgString);
	*((int*)(newString + bucketSize * sizeof(int))) = strlen(OrgString) + 1;

	int i,j;
	int location, idval;

	for(i = 0; i < newOrgLen; i++)
	{
		if(newOrgString[i] >= 'A' && newOrgString[i] <= 'Z')
			newOrgString[i] -= 'A' - 'a';
	}

	Queue queue(gramNum + 1);

	int base = 1 << (maxBit + 1);

	int* idArray;
	int* hashArray;

	int arraySize = newOrgLen - gramSize + 1;
	if(arraySize > 0)
	{
		idArray = new int[newOrgLen - gramSize + 1];
		hashArray = new int[newOrgLen - gramSize + 1];
	}

	for(i = 0; i < arraySize; i++)
	{
		idArray[i] = i;

		hashArray[i] = HashFunction::GetStringHashValue(newOrgString + i, gramSize, base);

		/*
		InvertedList* gramNode = list->SearchGram(newOrgString + i);

		if(gramNode)
			hashArray[i] = list->SearchGram(newOrgString+i)->GetTotalCount();
		else
			hashArray[i] = 0;

		if(hashArray[i] >= (1 << maxBit))
			continue;
		*/

		//cout << idArray[i] << " " << hashArray[i] << endl;

		queue.Enqueue(idArray + i, (double)(-1)*hashArray[i]);

		if(queue.GetCurSize() == (gramNum + 1))
		{
			queue.Dequeue();
			//queue.Print();
		}
	}

	void** resArray = queue.GetCurArray();
	int resSize = queue.GetCurSize();


	for(i = 0; i < gramNum; i++)
	{
		if(i < resSize)
		{
			location = *((int*) (resArray[i]));
			idval = hashArray[location];
			*((int*)(newString + (2 * i) * sizeof(int))) = idval + 1;
			*((int*)(newString + (2 * i + 1) * sizeof(int))) = location + 1;
		}
		else
		{
			//*((int*)(newString + (2 * i) * sizeof(int))) = (1 << (maxBit + 1)) - 1;
			//*((int*)(newString + (2 * i + 1) * sizeof(int))) = (1 << (maxBit+1))-1;
			*((int*)(newString + (2 * i) * sizeof(int))) = 1;
			*((int*)(newString + (2 * i + 1) * sizeof(int))) = 1;
		}

		//cout << *((int*)(newString + 2 * i * sizeof(int))) << "-" << 
		//	*((int*)(newString + (2 * i + 1) * sizeof(int))) << " ";
	}
	//cout << endl;

	for(i = 0; i < gramNum - 1; i++)
	{
		for(j = gramNum - 1; j > i; j--)
		{
			int tempLoc = *((int*)(newString + (2 * j + 1) * sizeof(int)));
			int tempHash = *((int*)(newString + (2 * j) * sizeof(int)));
			int tempPreLoc = *((int*)(newString + (2 * j - 1) * sizeof(int)));
			int tempPreHash = *((int*)(newString + (2 * j - 2) * sizeof(int)));

			if(tempLoc > tempPreLoc)
			//if(CompareGram(tempLoc, tempHash, tempPreLoc, tempPreHash))
			{
				//int tempHash = *((int*)(newString + (2 
				//	* j + 1) * sizeof(int)));

				*((int*)(newString + 2 * j * sizeof(int))) = 
					*((int*)(newString + (2 * j - 2) * sizeof(int)));
				*((int*)(newString + (2 * j + 1) * sizeof(int))) = 
					*((int*)(newString + (2 * j - 1) * sizeof(int)));
				*((int*)(newString + (2 * j - 2) * sizeof(int))) = 
					tempHash;
				*((int*)(newString + (2 * j - 1) * sizeof(int))) = 
					tempLoc;
			}
		}

		//for(j = 0; j < gramNum; j++)
		//	cout << *((int*)(newString + 2 * j * sizeof(int))) << "-" << 
		//	*((int*)(newString + (2 * j + 1) * sizeof(int))) << " ";
		//cout << endl;
	}

	/*
	ofstream gramfile("gram.txt", ios::app);

	for(i = 0; i < gramNum; i++)
	{
		int location = *((int*)(newString + 2 * i * sizeof(int)));
		int hashval  = *((int*)(newString + (2 * i + 1) * sizeof(int)));

		//if(location < 0 || hashval < 0)
			gramfile << location << "-" << hashval << " ";
	}
	gramfile << endl;
	*/

	strcpy(newString + bucketVol, OrgString);

	if(arraySize > 0)
	{
		delete [] idArray;
		delete [] hashArray;
	}

	return newOrgLen;

}

char* GramLocOrder::PrintString(char* pString)
{
	return pString + bucketVol;
}

bool GramLocOrder::CompareGram(int hval, int loc, int preHval, int preLoc, 
							   int* lbVal, int* ubVal, int* lbLoc, int* ubLoc,
							   int bitpos)
{
	int bit;
	int pBit;

	bit = loc >> bitpos;
	pBit = preLoc >> bitpos;

	if( bit > pBit )
		return true;
	if( bit < pBit )
		return false;

	if(bit % 2 == 0)
	{
		*ubLoc -= (1 << bitpos);
	}
	else
	{
		*lbLoc += (1 << bitpos);
	}

	bit = hval >> bitpos;
	pBit = preHval >> bitpos;

	if( bit > pBit )
		return true;
	if( bit < pBit )
		return false;

	if(bit % 2 == 0)
	{
		*ubVal -= (1 << bitpos);
	}
	else
	{
		*lbVal += (1 << bitpos);
	}

	return false;
}

bool GramLocOrder::CompareGram(int hval, int loc, int preHval, int preLoc)
{
	int bitpos;

	for(bitpos = maxBit; bitpos >= 0; bitpos--)
	{
		if( (loc >> bitpos) > (preLoc >> bitpos) )
			return true;
		if( (loc >> bitpos) < (preLoc >> bitpos) )
			return false;

		if( (hval >> bitpos) > (preHval >> bitpos) )
			return true;
		if( (hval >> bitpos) < (preHval >> bitpos) )
			return false;


	}

	return false;
}
