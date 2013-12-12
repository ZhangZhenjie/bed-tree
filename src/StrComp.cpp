#include "StrComp.h"

#include <cstring>
#include <cmath>

StrComparator::StrComparator(int length)
{
	distCompTime = lbCompTime = 0;

	maxSize = length;

	matrix = new int*[maxSize + 1];

	int i;

	for(i = 0; i < maxSize + 1; i++)
	{
		matrix[i] = new int[maxSize + 1];
	}

	minString[0] = '\0';
	maxString[0] = '\0';
}

StrComparator::~StrComparator()
{
	int i;

	for(i = 0; i < maxSize + 1 ; i++)
	{
		delete [] matrix[i];
	}

	delete [] matrix;
}

bool StrComparator::isBefore(char* str1, char* str2)
{
	return true;
}

bool StrComparator::isEqual(char* str1, char* str2)
{
	return true;
}

bool StrComparator::isNotAfter(char* st1, char* str2)
{
	return true;	
}

float StrComparator::LowerBound(char* lsString, char* leString, char* rsString, char* reString, float threshold)
{
	return 0;
}


float StrComparator::LowerBound(char* qString, int qStrLen, char* sString, char* eString,
							  float threshold)
{
	//naive bound
	return 0;
}

int StrComparator::UpperBound(char* qString, char* sString, char* eString)
{
	//naive bound
	return strlen(qString);
}

float StrComparator::Dist(char* qString, char* tString, float threshold)
{
	distCompTime++;

	qString = RemoveHead(qString);
	tString = RemoveHead(tString);

	int qStrLen = strlen(qString);
	int tStrLen = strlen(tString);

	int lenDiff = (qStrLen-tStrLen)>0?(qStrLen-tStrLen):(tStrLen-qStrLen);

	if(lenDiff > threshold)
		return lenDiff;

	int i,j;

	
	//////////////////////////////////////////////////////////
	// letter counting optimization starts here
	//////////////////////////////////////////////////////////
	for(i = 0; i < 26; i++)
		//tLetterCounter[i] = 0;
		tLetterCounter[i] = qLetterCounter[i] = 0;

	for(i = 0; i < tStrLen; i++){
		int pos = tString[i] - 'a';

		if(pos >= 0 && pos < 26)
			tLetterCounter[pos]++;
	}
	for(i = 0; i < qStrLen; i++){
		int pos = qString[i] - 'a';

		if(pos >= 0 && pos < 26)
			qLetterCounter[pos]++;
	}

	int exc_sum = 0;
	int bel_sum = 0;
	for(i = 0; i < 26; i++)
	{
		if(qLetterCounter[i] > tLetterCounter[i])
		{
			exc_sum += qLetterCounter[i] - tLetterCounter[i];
			if(exc_sum > threshold)
				return exc_sum;
		}
		else{
			bel_sum += tLetterCounter[i] - qLetterCounter[i];
			if(bel_sum > threshold)
				return bel_sum;
		}
	}
	///////////////////////////////////////////////////////
	// letter counting optimization ends here
	///////////////////////////////////////////////////////
	



	int min;

	for(i = 0; i < tStrLen + 1; i++)
	{
		if(i == 0)
		{
			for(j = 0; j < qStrLen + 1; j++)
			{
				matrix[i][j] = j;
#ifdef _DEBUG_
				cout << matrix[i][j] << " ";
#endif
			}

#ifdef _DEBUG_
			cout << endl;
#endif

			continue;
		}

		matrix[i][0] = i;

		min = qStrLen;

#ifdef _DEBUG_
		cout << matrix[i][0] << " ";
#endif
		int startPos = (i - ((int)threshold + 1)) > 1 ? (i - ((int)threshold+1)) : 1;
		//int startPos = 1;
		int endPos = (i + ((int)threshold + 1)) < qStrLen ? (i + ((int)threshold + 1)) : qStrLen;
		//int endPos = qStrLen;

		if(startPos > 1)
		{
			matrix[i][startPos-1] = matrix[i-1][startPos - 1] < matrix[i-1][startPos-2] ?
				(matrix[i-1][startPos-1] + 1) : (matrix[i-1][startPos-2] + 1);
		}

		for(j = startPos; j <= endPos; j++)
		{
			if(tString[i - 1] == qString[j - 1])
				matrix[i][j] = matrix[i - 1][j - 1];
			else
				matrix[i][j] = matrix[i - 1][j - 1] + 1;

			if( (matrix[i][j - 1] + 1) < matrix[i][j])
				matrix[i][j] = matrix[i][j - 1] + 1;

			if( (matrix[i - 1][j] + 1) < matrix[i][j])
				matrix[i][j] = matrix[i - 1][j] + 1;
#ifdef _DEBUG_
			cout << matrix[i][j] << " ";
#endif
			if(matrix[i][j] < min)
				min = matrix[i][j];
		}
		if(j <= qStrLen)
		{
			matrix[i][j] = matrix[i][j - 1] < matrix[i - 1][j - 1] ?
				(matrix[i][j - 1] + 1) : (matrix[i - 1][j - 1] + 1);
		}


		if(min > threshold)
			return min;

#ifdef _DEBUG_
		cout << endl;
#endif	
	}

	return matrix[tStrLen][qStrLen];
}

/*
int StrComparator::Dist(char* qString, char* tString, int threshold)
{
	distCompTime++;

	qString = RemoveHead(qString);
	tString = RemoveHead(tString);

	int qStrLen = strlen(qString);
	int tStrLen = strlen(tString);

	int i,j;

	int min;

	for(i = 0; i < tStrLen + 1; i++)
	{
		if(i == 0)
		{
			for(j = 0; j < qStrLen + 1; j++)
			{
				matrix[i][j] = j;
#ifdef _DEBUG_
				cout << matrix[i][j] << " ";
#endif
			}

#ifdef _DEBUG_
			cout << endl;
#endif

			continue;
		}

		matrix[i][0] = i;

		min = qStrLen;

#ifdef _DEBUG_
		cout << matrix[i][0] << " ";
#endif

		for(j = 1; j < qStrLen + 1; j++)
		{
			if(tString[i - 1] == qString[j - 1])
				matrix[i][j] = matrix[i - 1][j - 1];
			else
				matrix[i][j] = matrix[i - 1][j - 1] + 1;

			if( (matrix[i][j - 1] + 1) < matrix[i][j])
				matrix[i][j] = matrix[i][j - 1] + 1;

			if( (matrix[i - 1][j] + 1) < matrix[i][j])
				matrix[i][j] = matrix[i - 1][j] + 1;
#ifdef _DEBUG_
			cout << matrix[i][j] << " ";
#endif
			if(matrix[i][j] < min)
				min = matrix[i][j];
		}

		if(min > threshold)
			return min;

#ifdef _DEBUG_
		cout << endl;
#endif	
	}

	return matrix[tStrLen][qStrLen];
}
*/

void StrComparator::QueryInfo(char* qString, int qStrLen)
{
	qString = RemoveHead(qString);

	int i;

	for(i = 0; i < 26; i++)
		qLetterCounter[i] = 0;

	for(i = 0; i < qStrLen; i++){
		int pos = qString[i] - 'a';

		if(pos >= 0 && pos < 26)
			qLetterCounter[pos]++;
	}
}

float StrComparator::Dist(char* qString, int qStrLen, char* tString, float threshold)
{
	distCompTime++;

	qString = RemoveHead(qString);
	tString = RemoveHead(tString);

	int tStrLen = strlen(tString);

	int lenDiff=(qStrLen-tStrLen)>0?(qStrLen-tStrLen):(tStrLen-qStrLen);

	if(lenDiff > threshold)
		return lenDiff;

	int i,j;

	
	//////////////////////////////////////////////////////////
	// letter counting optimization starts here
	//////////////////////////////////////////////////////////
	for(i = 0; i < 26; i++)
		tLetterCounter[i] = 0;
		//tLetterCounter[i] = qLetterCounter[i] = 0;

	for(i = 0; i < tStrLen; i++){
		int pos = tString[i] - 'a';

		if(pos >= 0 && pos < 26)
			tLetterCounter[pos]++;
	}
	
	int exc_sum = 0;
	int bel_sum = 0;
	for(i = 0; i < 26; i++)
	{
		if(qLetterCounter[i] > tLetterCounter[i])
		{
			exc_sum += qLetterCounter[i] - tLetterCounter[i];
			if(exc_sum > threshold)
				return exc_sum;
		}
		else{
			bel_sum += tLetterCounter[i] - qLetterCounter[i];
			if(bel_sum > threshold)
				return bel_sum;
		}
	}
	///////////////////////////////////////////////////////
	// letter counting optimization ends here
	///////////////////////////////////////////////////////
	



	int min;

	for(i = 0; i < tStrLen + 1; i++)
	{
		if(i == 0)
		{
			for(j = 0; j < qStrLen + 1; j++)
			{
				matrix[i][j] = j;
#ifdef _DEBUG_
				cout << matrix[i][j] << " ";
#endif
			}

#ifdef _DEBUG_
			cout << endl;
#endif

			continue;
		}

		matrix[i][0] = i;

		min = qStrLen;

#ifdef _DEBUG_
		cout << matrix[i][0] << " ";
#endif
		int startPos = (i - ((int)threshold + 1)) > 1 ? (i - ((int)threshold + 1)) : 1;
		//int startPos = 1;
		int endPos = (i + ((int)threshold + 1)) < qStrLen ? (i + ((int)threshold + 1)) : qStrLen;
		//int endPos = qStrLen;

		if(startPos > 1)
		{
			matrix[i][startPos-1] = matrix[i-1][startPos - 1] < matrix[i-1][startPos-2] ?
				(matrix[i-1][startPos-1] + 1) : (matrix[i-1][startPos-2] + 1);
		}

		for(j = startPos; j <= endPos; j++)
		{
			if(tString[i - 1] == qString[j - 1])
				matrix[i][j] = matrix[i - 1][j - 1];
			else
				matrix[i][j] = matrix[i - 1][j - 1] + 1;

			if( (matrix[i][j - 1] + 1) < matrix[i][j])
				matrix[i][j] = matrix[i][j - 1] + 1;

			if( (matrix[i - 1][j] + 1) < matrix[i][j])
				matrix[i][j] = matrix[i - 1][j] + 1;
#ifdef _DEBUG_
			cout << matrix[i][j] << " ";
#endif
			if(matrix[i][j] < min)
				min = matrix[i][j];
		}
		if(j <= qStrLen)
		{
			matrix[i][j] = matrix[i][j - 1] < matrix[i - 1][j - 1] ?
				(matrix[i][j - 1] + 1) : (matrix[i - 1][j - 1] + 1);
		}


		if(min > threshold)
			return min;

#ifdef _DEBUG_
		cout << endl;
#endif	
	}

	return matrix[tStrLen][qStrLen];
}

bool StrComparator::isSameChar(char* sString, int sStrLen, char* eString, int eStrLen, 
							  int index)
{
	if(index >= sStrLen)
		return false;

	if(index >= eStrLen)
		return false;

	return (sString[index] == eString[index]);
}

bool StrComparator::isBetween(char* sString, char* eString, int pos, char* qString, int qPos)
{
	char sChar = sString[pos];
	char eChar = eString[pos];

	char qChar = qString[qPos];

	if(sChar != '\0' && qChar < sChar)
		return false;

	if(eChar != '\0' && qChar > eChar)
		return false;

	return true;
}

char* StrComparator::RemoveHead(char* str)
{
	if(*str == '\0')
		return str;

	while( *str != ' ')
		str++;

	while( *str == ' ')
		str++;

	return str;
}

int StrComparator::PreProcessing(char* orgString, char* newString)
{
	strcpy(newString, orgString);

	return strlen(newString);
}

int StrComparator::OrgProcessing(char* orgString, char* newString)
{
	strcpy(newString, orgString);

	return strlen(newString);
}

char* StrComparator::PrintString(char* pString)
{
	return pString;
}


void StrComparator::CompressString(char* mystr)
{
}


void StrComparator::PrintHead(ofstream& output, char* mystr)
{
}
