#include "HashFunction.h"
#include <math.h>

HashFunction::HashFunction()
{
}


HashFunction::~HashFunction()
{
}

int HashFunction::GetBitHashValue(int v)
{
	// a simple implementation of the hash function
	// can be replaced with something better if necessary
	int shift = v % 32;

	return (1 << shift);
}

int HashFunction::GetBinaryHashValue(int v1, int v2, int mod)
{
	int retValue = (v1 * 271 * 257 * 241 + v2 * 263 * 269 * 251) % mod;
	if(retValue > 0)
		return retValue;
	else
		return mod + retValue;
}

int HashFunction::GetUniHashValue(int v, int mod)
{
	int retValue = (v * 271 * 257 * 241) % mod;
	if(retValue > 0)
		return retValue;
	else
		return mod + retValue;
}

int HashFunction::GetStringHashValue(char* str, int len, int mod)
{
	int i;

	int hashPos = 0;

	for(i = 0; i < len; i++)
	{
		hashPos = (hashPos + (i + 1) * GetUniHashValue(str[i], mod)) % mod;
	}

	if(hashPos < 0)
		hashPos = mod + hashPos;

	return hashPos;
}

/*
int HashFunction::GetStringSignedHashValue(char* str, int len, int mod)
{
	int i;

	int hashPos = 0;

	for(i = 0; i < len; i++)
	{
		hashPos = (hashPos + GetUniHashValue(str[i], mod)) % mod;
	}

	//if(hashPos < 0)
	//	hashPos = mod + hashPos;

	return hashPos;
}
*/

