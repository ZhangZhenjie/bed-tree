#include "StrDictOrder.h"
#include <cstring>
#include <iostream>

using namespace std;

StrDictOrder::StrDictOrder(int length) : StrComparator(length)
{
	strcpy(minString, "100 ");
	strcpy(maxString, "100 ");
}

StrDictOrder::~StrDictOrder()
{
}

bool StrDictOrder::isBefore(char* str1, char* str2)
{
	str1 = RemoveHead(str1);

	str2 = RemoveHead(str2);

	int len1 = strlen(str1);
	int len2 = strlen(str2);

	if(len1 == 0 && len2 == 0)
		return false;

	if(len1 == 0)
		return true;

	if(len2 == 0)
		return false;

	//get the length of the shorter string
	int minLen = len1 < len2 ? len1 : len2;

	int i;

	for(i = 0; i < minLen; i++)
	{
		if(str1[i] < str2[i])
			return true;
		else if(str2[i] < str1[i])
			return false;
	}

	if(len1 < len2)
		return true;

	return false;
}

bool StrDictOrder::isEqual(char* str1, char* str2)
{
	str1 = RemoveHead(str1);

	str2 = RemoveHead(str2);

	int len1 = strlen(str1);
	int len2 = strlen(str2);

	if(len1 != len2)
		return false;

	int i;

	for(i = 0; i < len1; i++)
	{
		if(str1[i] != str2[i])
			return false;
	}

	return true;
}

bool StrDictOrder::isNotAfter(char* str1, char* str2)
{
	return !isBefore(str2, str1);
}

float StrDictOrder::LowerBound(char* lsString, char* leString, char* rsString, char* reString, float threshold)
{
	lbCompTime++;

	char* sString = RemoveHead(lsString);
	char* eString = RemoveHead(leString);

	int sStrLen = strlen(sString);
	int eStrLen = strlen(eString);

	int maxLen = sStrLen < eStrLen ? sStrLen : eStrLen;
	int comStrLen = 0;

	while(comStrLen < maxLen)
	{
		if(sString[comStrLen] == eString[comStrLen])
			comStrLen++;
		else
			break;
	}

	float lb1;

	if(comStrLen > 0)
		lb1 = LowerBound(lsString, comStrLen, rsString, reString, threshold);
	else
		lb1 = 0;

	sString = RemoveHead(rsString);
	eString = RemoveHead(reString);

	sStrLen = strlen(sString);
	eStrLen = strlen(eString);
	
	maxLen = sStrLen < eStrLen ? sStrLen : eStrLen;
	comStrLen = 0;

	while(comStrLen < maxLen)
	{
		if(sString[comStrLen] == eString[comStrLen])
			comStrLen++;
		else
			break;
	}

	float lb2;

	if(comStrLen > 0)
		lb2 = LowerBound(lsString, comStrLen, rsString, reString, threshold);
	else
		lb2 = 0;

	return lb1 < lb2 ? lb1 : lb2;
}

float StrDictOrder::LowerBound(char* qString, int qStrLen, char* sString, char* eString, 
							 float threshold = 5000)
{
	lbCompTime++;

	qString = RemoveHead(qString);
	sString = RemoveHead(sString);
	eString = RemoveHead(eString);

	int sStrLen = strlen(sString);
	int eStrLen = strlen(eString);

	//int testLen = sStrLen < eStrLen ? sStrLen : eStrLen;

	//int qStrLen = strlen(qString);

	int row, i;

	bool prefixFlag = true;

	int min;

	for(row = 0; row < qStrLen + 1; row++)
	{
		int pos = row - 1;

		if(!prefixFlag)
			break;

		if(row == 0)
		{
			for(i = 0; i < qStrLen + 1; i++)
			{
				matrix[row][i] = i;
			}

			continue;
		}

		matrix[row][0] = row;

		min = qStrLen;

		prefixFlag = isSameChar(sString, sStrLen, eString, eStrLen, pos);

		for(i = 1; i < qStrLen + 1; i++)
		{
			if(isBetween(sString, eString, pos, qString, i - 1))
				matrix[row][i] = matrix[row - 1][i - 1];
			else
				matrix[row][i] = matrix[row - 1][i - 1] + 1;
				
			if( (matrix[row][i - 1] + 1) < matrix[row][i])
				matrix[row][i] = matrix[row][i - 1] + 1;

			if( (matrix[row - 1][i] + 1) < matrix[row][i])
				matrix[row][i] = matrix[row - 1][i] + 1;

			if(matrix[row][i] < min)
				min = matrix[row][i];
		}

		if(min > threshold)
			break;
	}

	return min;
}

int StrDictOrder::UpperBound(char* qString, char* sString, char* eString)
{

	return strlen(qString);
}

int StrDictOrder::PreProcessing(char* OrgString, char* newString)
{
	strcpy(newString, OrgString);

	int newLen = strlen(newString);

	int i;

	for(i = 0; i < newLen; i++)
	{
		if(newString[i] >= 'A' && newString[i] <= 'Z')
			newString[i] -= 'A' - 'a';
	}

	char* tempstr = newString;

	if(*tempstr >= '0' && *tempstr <= '9')
	{
		while(*tempstr != ' ')
		{
			tempstr++;
			newLen--;
		}

		while(*tempstr == ' ')
		{
			tempstr++;
			newLen--;
		}
	}

	return newLen;
}

int StrDictOrder::OrgProcessing(char* OrgString, char* newString)
{
	return PreProcessing(OrgString, newString);
}

char* StrDictOrder::PrintString(char* pString)
{
	return pString;
}
