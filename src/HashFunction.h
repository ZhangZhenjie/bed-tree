#ifndef _HASH_H_
#define _HASH_H_

class HashFunction
{
private:

public:
	HashFunction();
	~HashFunction();

	static int GetBitHashValue(int v);

	static int GetBinaryHashValue(int v1, int v2, int mod);
	static int GetUniHashValue(int v, int mod);

	static int GetStringHashValue(char* str, int len, int mod);
	//static int GetStringSignedHashValue(char* str, int len, int mod);
};

#endif
