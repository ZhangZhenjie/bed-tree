#ifndef _STR_B_PLUS_TREE_
#define _STR_B_PLUS_TREE_

#include "Node.h"
#include "StrComp.h"

#include <time.h>


class SBPTree
{
public:
	SBPTree(int pSize, BufferManager* manager, StrComparator* comp);
	~SBPTree();

	bool InsertString(char* newString);

	// verify the tree structure, for testing only
	void Verify(bool isPrint);

	int RangeQuery(char* queryString, int qStrLen, float threshold);

	int TopKQuery(char* queryString, int qStrLen, int k);

	int SelfJoinQuery(float threshold);

	int TopKSelfJoinQuery(int k);

private:
	//structure parameters
	int pageSize;

	//current status parameters
	int curStrNum;

	Node* root;

	StrComparator* strcomp;
	BufferManager* bm;

	//statistics
	int queryTime;
	int insertTime;
	int deleteTime;
};

#endif

