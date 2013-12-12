#include "InvertedList.h"

#include <cstring>
#include <iostream>

#include "HashFunction.h"

using namespace std;

InvertedList::InvertedList(int bSize, int gSize, int length, StrComparator* comp,
						   BufferManager* manager, char* mystring, 
						   InvertedList* lSibling, InvertedList* rSibling,
						   InvertedList* mychild) : Page(bSize, manager)
{
	maxLength = length;
	gramSize = gSize;

	hashArraySize = 0;

	hashString = new char[gramSize + 1];

	int i;

	for(i = 0; i < gramSize; i++)
		hashString[i] = mystring[i];
	hashString[gramSize] = '\0';

	//strcpy(hashString, mystring);

	leftSibling = lSibling;
	rightSibling = rSibling;
	child = mychild;

	longSize = sizeof(long);

	maxSize = pageSize / longSize;
	curSize = 0;

	strcomp = comp;

	nodeID = manager->RequestID();
	manager->AddEntry(this, nodeID);

	totalSize = totalCount = 0;
}

InvertedList::InvertedList(int bSize, int gSize, int length, StrComparator* comp, 
						   BufferManager* manager) : Page(bSize, manager)
{
	maxLength = length;
	gramSize = gSize;

	hashArraySize = 100000;
	//hashArray = new long[hashArraySize];

	listHashArray = NULL;
	strPosArray = NULL;

	hashString = NULL;
	rightSibling = NULL;
	leftSibling = NULL;
	child = NULL;

	longSize = sizeof(long);
	maxSize = 0;
	curSize = 0;
	nodeID = -1;

	strcomp = comp;
}

InvertedList::~InvertedList()
{
	if(hashArraySize > 0)
	{
		if(listHashArray)
			delete [] listHashArray;
		if(strPosArray)
			delete [] strPosArray;
	}
	else
		delete [] hashString;
}

/*
void InvertedList::GetDF(char* filename, int lineNum)
{

	int i,j,l;

	for(i = 0; i < hashArraySize; i++)
	{
		hashArray[i]  = 0;
	}

	char* orgbuffer = new char[maxLength];
	//char* curstr = new char[maxLength];
	int* countArray = new int[maxLength];
	
	input.open(filename);

	for(i = 0; i < lineNum; i++)
	{
		input.getline(orgbuffer, maxLength);

		//strcomp->PreProcessing(buffer, curstr);

		//cout << orgbuffer << endl;

		char* tbuffer = strcomp->RemoveHead(orgbuffer);

		int curlen = strlen(tbuffer);

		for(j = 0; j < curlen; j++)
		{
			countArray[j] = HashFunction::GetUniHashValue(tbuffer[j], hashArraySize);
		}

		//for(j = 1; j < curlen; j++)
		//{
		//	countArray[j] = (countArray[j] + countArray[j-1]) % hashArraySize;
		//}

		for(j = 0; j <= curlen - gramSize; j++)
		{
			int startPos = j;
			int endPos = j + gramSize - 1;

			//int factor = 1;
			int hashPos;
			for(l = 0; l < gramSize; l++)
			{
				hashPos = (hashPos + (l + 1) * countArray[startPos + l]) % hashArraySize;
			}

			//int hashPos = (countArray[endPos] - countArray[startPos]) % hashArraySize;

			if(hashPos < 0)
				hashPos = hashArraySize + hashPos;

			
			cout << i  << " " << j << " ";
			for(l = 0; l < gramSize; l++)
				cout << buffer[startPos + l];
			
			cout << " : " << hashPos << endl;
			

			hashArray[hashPos]++;
		}
	}

	int zeroCount = 0;

	
	for(i = 0; i < hashArraySize; i++)
	{
		if(hashArray[i] == 0)
			zeroCount++;
	}
	

	cout << zeroCount << endl;
	delete [] countArray;
	//delete [] curstr;
	delete [] orgbuffer;

	input.close();
}
*/

void InvertedList::ConstructCount(char* filename, int lineNum)
{
	input.open(filename);

	int i,j;

	listHashArray = new InvertedList*[hashArraySize];

	for(i = 0; i < hashArraySize; i++)
		listHashArray[i] = NULL;

	char* stringbuf = new char[maxLength + 1];
	char* orgstrbuf = new char[maxLength + 1];

	for(i = 0; i < lineNum; i++)
	{
		int hashPos;

		input.getline(orgstrbuf, maxLength);

		strcomp->PreProcessing(orgstrbuf, stringbuf);

		char* tempstr = strcomp->RemoveHead(stringbuf);

		int tempstrlen = strlen(tempstr);

		for(j = 0; j <= tempstrlen - gramSize; j++)
		{
			hashPos = HashFunction::GetStringHashValue(tempstr+j, gramSize, hashArraySize);

			InvertedList* gramNode = listHashArray[hashPos];

			if(gramNode)
			{
				gramNode = listHashArray[hashPos]->SearchGram(tempstr+j);
			}

			if(!gramNode)
			{
				gramNode = new InvertedList(pageSize, gramSize, maxLength,
					strcomp, bm, tempstr + j, NULL, listHashArray[hashPos], NULL);

				if(listHashArray[hashPos])
					listHashArray[hashPos]->leftSibling = gramNode;
				
				listHashArray[hashPos] = gramNode;
			}

			gramNode->CountID( (long)i );
		}
	}

	delete [] orgstrbuf;
	delete [] stringbuf;

	input.close();
}

void InvertedList::ConstructIndex(char* filename, int lineNum, int threshold)
{
	totalSize = lineNum;

	input.open(filename);

	strPosArray = new long[lineNum];

	int i,j;

	if(!listHashArray)
	{
		listHashArray = new InvertedList*[hashArraySize];

		for(i = 0; i < hashArraySize; i++)
			listHashArray[i] = NULL;
	}

	char* stringbuf = new char[maxLength + 1];
	char* orgstrbuf = new char[maxLength + 1];

	int targetGramSize;

	char** targetGramArray = new char*[maxLength - gramSize + 1];
	int* targetLocArray = new int[maxLength - gramSize + 1];
	long* targetWeightArray = new long[maxLength - gramSize + 1];

	for(i = 0; i < lineNum; i++)
	{
		strPosArray[i] = input.tellg();

		/*
		if(i > 0)
			strPosArray[i]--;
		input.seekg(strPosArray[i]);
		*/

		int hashPos;

		input.getline(orgstrbuf, maxLength);

		strcomp->PreProcessing(orgstrbuf, stringbuf);

		char* tempstr = strcomp->RemoveHead(stringbuf);

		int tempstrlen = strlen(tempstr);

		targetGramSize = tempstrlen - gramSize + 1;

		ParseString(tempstr, tempstrlen, targetGramArray, targetLocArray, targetWeightArray);

		int testGramNum = targetGramSize;

		if(threshold > 0)
		{
			testGramNum = gramSize * threshold + 1;
			if(testGramNum > targetGramSize)
				testGramNum = targetGramSize;

			testGramNum = FindMinPrefix(targetGramArray, targetLocArray, targetWeightArray,
				testGramNum, threshold);
		}

		//////////////////////////////////////////////////////////
		/*
		if(i == 458967)
		{
			for(j = 0; j < testGramNum; j++)
			{
				for(int l = 0; l < gramSize; l++)
				{
					cout << *(targetGramArray[j] + l);
				}
				cout << endl;
			}
		}
		*/
		//////////////////////////////////////////////////////////


		for(j = 0; j < testGramNum; j++)
		{
			//PrintGram(tempstr + j);

			hashPos = HashFunction::GetStringHashValue(targetGramArray[j], 
				gramSize, hashArraySize);

			InvertedList* gramNode = listHashArray[hashPos];

			if(gramNode)
			{
				gramNode = listHashArray[hashPos]->SearchGram(targetGramArray[j]);
			}

			if(!gramNode)
			{
				gramNode = new InvertedList(pageSize, gramSize, maxLength,
					strcomp, bm, targetGramArray[j], NULL, listHashArray[hashPos], NULL);

				if(listHashArray[hashPos])
					listHashArray[hashPos]->leftSibling = gramNode;
				
				listHashArray[hashPos] = gramNode;
			}

			gramNode->InsertID( (long)i );
		}
	}

	delete [] orgstrbuf;
	delete [] stringbuf;

	delete [] targetGramArray;
	delete [] targetLocArray;
	delete [] targetWeightArray;
}

void InvertedList::InsertID(long id)
{
	if(curSize >= maxSize)
	{
		InvertedList* gramNode = new InvertedList(pageSize, gramSize, maxLength,
			strcomp, bm, hashString, NULL, NULL, child);
		child = gramNode;

		LoadFromFile();

		memcpy(gramNode->buffer, buffer, pageSize);

		gramNode->curSize = maxSize;
		curSize = 0;

		//cout << "Copying from node " << nodeID << " to node " << gramNode->nodeID << endl;
	}

	LoadFromFile();

	int pos = longSize * curSize;

	*((long*)(buffer+pos)) = id;

	curSize++;
	totalSize++;

	bm->WriteUpdate(nodeID);
}

void InvertedList::CountID(long id)
{
	totalCount++;
}

InvertedList* InvertedList::SearchGram(char* gramStr)
{
	if(hashString == NULL)
	{
		//cout << "hash string should not be empty" << endl;
		int hashPos = HashFunction::GetStringHashValue(gramStr, 
			gramSize, hashArraySize);

		InvertedList* gramNode = listHashArray[hashPos];

		if(gramNode)
		{
			gramNode = listHashArray[hashPos]->SearchGram(gramStr);
		}

		return gramNode;
	}

	int i;

	for(i = 0; i < gramSize; i++)
	{
		if(hashString[i] != gramStr[i])
			break;
	}

	if(i == gramSize)
		return this;

	if(rightSibling == NULL)
		return NULL;

	return rightSibling->SearchGram(gramStr);
}

void InvertedList::PrintGram(char* gramStr)
{
	int i;

	for(i = 0; i < gramSize; i++)
	{
		cout << *(gramStr+i);
	}

	cout << endl;
}

int InvertedList::PrintIDs()
{
	LoadFromFile();
	bm->ReadUpdate(nodeID);

	int sum = 0;

	if(child)
	{
		sum += child->PrintIDs();
	}

	int i;

	for(i = 0; i < curSize; i++)
	{
		int pos = longSize * i;

		cout << *((long*)(buffer + pos)) << " ";
	}

	cout << endl;

	return (sum + curSize);
}

int InvertedList::PrintAllIDs()
{
	//if(!isInMem)
	//	LoadFromFile();
	//bm->ReadUpdate(nodeID);

	int sum = 0;

	if(rightSibling)
	{
		sum += rightSibling->PrintAllIDs();
	}

	cout << hashString << " : " << endl;
	sum += PrintIDs();
	//cout << endl;

	return sum;
}

void InvertedList::ParseString(char* mystring, int mystrlen, char** gramArray, 
							   int* locArray, long* weightArray)
{
	int i,j;

	//retrieve the q-grams including location and weight
	for(i = 0; i <= mystrlen - gramSize; i++)
	{
		gramArray[i] = mystring + i;

		locArray[i] = i;

		int hashPos = HashFunction::GetStringHashValue(gramArray[i], gramSize, hashArraySize);

		InvertedList* hashNode = listHashArray[hashPos]->SearchGram(gramArray[i]);

		weightArray[i] = hashNode->totalCount;

		//PrintGram(gramArray[i]);
		//cout << locArray[i] << " " << weightArray[i] << endl;
	}

	//sort these q-grams

	int curGramNum = mystrlen - gramSize + 1;

	for(i = 0; i < curGramNum - 1; i++)
	{
		for(j = curGramNum - 1; j > i; j--)
		{
			if(weightArray[j] < weightArray[j - 1])
			{
				//swap
				char* tempGram = gramArray[j];
				int tempLoc = locArray[j];
				int tempWeight = weightArray[j];

				gramArray[j] = gramArray[j - 1];
				locArray[j] = locArray[j - 1];
				weightArray[j] = weightArray[j - 1];

				gramArray[j - 1] = tempGram;
				locArray[j - 1] = tempLoc;
				weightArray[j - 1] = tempWeight;
			}
		}
	}
}

int InvertedList::RangeQuery(char* queryString, int threshold)
{
	int i;

	char* orgQueryString = queryString;

	queryString = strcomp->RemoveHead(queryString);

	int queryStrLen = strlen(queryString);

	int queryGramSize = queryStrLen - gramSize + 1;
	char** queryGramArray = new char*[queryGramSize];
	int* queryLocArray = new int[queryGramSize];
	long* queryWeightArray = new long[queryGramSize];

	//int targetGramSize;

	char** targetGramArray = new char*[maxLength - gramSize + 1];
	int* targetLocArray = new int[maxLength - gramSize + 1];
	long* targetWeightArray = new long[maxLength - gramSize + 1];

	char* targetBuffer = new char[maxLength];
	char* myString = new char[maxLength];
	
	bool* checkList = new bool[totalSize];

	for(i = 0; i < totalSize; i++)
		checkList[i] = false;

	ParseString(queryString, queryStrLen, queryGramArray, 
		queryLocArray, queryWeightArray);

	/*
	for(i = 0; i < queryGramSize; i++)
	{
		PrintGram(queryGramArray[i]);
		cout << queryLocArray[i] << " " << queryWeightArray[i] << endl;
	}
	*/
	
	int resSize = 0;

	//we can replace it with the new method in vldb 2008
	int testGramNum = gramSize * threshold + 1;
	if(testGramNum > queryGramSize)
		testGramNum = queryGramSize;

	testGramNum = FindMinPrefix(queryGramArray, queryLocArray, queryWeightArray,
		testGramNum, threshold);

	/*
	for(i = 0; i < testGramNum; i++)
	{
		for(int l = 0; l < gramSize; l++)
		{
			cout << *(queryGramArray[i] + l);
		}
		cout << endl;
	}
	*/

	for(i = 0; i < testGramNum; i++)
	{
		int hashPos = HashFunction::GetStringHashValue(queryGramArray[i], 
			gramSize, hashArraySize);

		InvertedList* hashNode = listHashArray[hashPos];

		if(hashNode)
			hashNode = hashNode->SearchGram(queryGramArray[i]);

		if(hashNode)
		{
			InvertedList* curNode = hashNode;

			int curPos = 0;

			curNode->LoadFromFile();
			bm->ReadUpdate(curNode->nodeID);

			for(long j = 0; j < hashNode->totalSize; j++)
			{
				long canID = *((long*)(curNode->buffer + curPos * longSize));

				if(!checkList[canID])
				{
					checkList[canID] = true;

					//if(canID == 266426)
					//	cout << endl;

					input.seekg(strPosArray[canID]);

					input.getline(targetBuffer, maxLength);

					strcomp->PreProcessing(targetBuffer, myString);
					//char* targetString = strcomp->RemoveHead(myString);

					/*
					int targetStrLen = strlen(targetString);
					targetGramSize = targetStrLen - gramSize + 1;

					//cout << targetString << endl;

					ParseString(targetString, targetStrLen, targetGramArray, 
						targetLocArray, targetWeightArray);

					*/


					// compare the two strings here
					float dist = strcomp->Dist(orgQueryString, myString, threshold);

					if(dist <= threshold)
					{
						//cout << targetBuffer << endl;
						resSize++;
					}
				}
				
				//move to next string
				if(curPos == curNode->curSize)
				{
					curNode = curNode->child;

					curNode->LoadFromFile();
					bm->ReadUpdate(curNode->nodeID);

					curPos = 0;
				}
				else
					curPos++;
			}
		}
	}


	delete [] targetBuffer;
	delete [] myString;

	delete [] queryGramArray;
	delete [] queryLocArray;
	delete [] queryWeightArray;

	delete [] targetGramArray;
	delete [] targetLocArray;
	delete [] targetWeightArray;

	delete [] checkList;

	return resSize;
}

int InvertedList::FindMinPrefix(char** gramArray, int* locArray, 
								long* weightArray, int testNum, int threshold)
{
	int i,j;

	for(i = 0; i < testNum; i++)
	{
		for(j = i; j > 0; j--)
		{
			if(locArray[j] < locArray[j - 1])
			{
				//swap
				char* tempGram = gramArray[j];
				int tempLoc = locArray[j];
				int tempWeight = weightArray[j];

				gramArray[j] = gramArray[j - 1];
				locArray[j] = locArray[j - 1];
				weightArray[j] = weightArray[j - 1];

				gramArray[j - 1] = tempGram;
				locArray[j - 1] = tempLoc;
				weightArray[j - 1] = tempWeight;
			}
			else
				break;
		}

		int count = 0;
		int preLoc = 0;

		for(j = 0; j <= i; j++)
		{
			if(locArray[j] > preLoc)
			{
				count++;
				preLoc = locArray[j] + gramSize - 1;

				if(count >= threshold)
					return (j + 1);
			}
		}
	}

	return testNum;
}
