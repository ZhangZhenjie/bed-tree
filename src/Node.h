#ifndef _NODE_H_
#define _NODE_H_

#include "StrComp.h"
#include "BufferManager.h"
#include "Page.h"
#include "Queue.h"

#include <fstream>

using namespace std;

class SimilarPair
{
public:
	string leftString;
	string rightString;

	SimilarPair(char* lstr, char* rstr);
	~SimilarPair();
};

class Node;

class Node : public Page
{
public:
	Node(int size, bool leaf, StrComparator* comp, BufferManager* manager);
	Node(int size, bool leaf, StrComparator* comp, Node* splitNode,
		BufferManager* manager);
	~Node();

	Node* InsertNewString(char* newString);
	Node* InsertNewEntry(Node* newNode, char* insertString);

	void PrintNode();

	int RangeQuery(char* queryString, int qStrLen, float threshold, 
		char* sString, char* eString);
	int TopKQuery(char* queryString, int qStrLen, Queue* q, int k, float& threshold,
		char* sString, char* eString);
	int CountSubtree(char* preString, bool isPrint);
	void ClearSubtree();

	Node* FindNode(char* queryString);
	int GetStrNum(){return curStrNum;};

	static int JoinQuery(Node* lNode, Node* rNode, float threshold, char* lsString, char* leString,
		char* rsString, char* reString);
	static int TopKJoinQuery(Node* lNode, Node* rNode, Queue* q, int k, float& threshold, char* lsString, char* leString,
		char* rsString, char* reString);
private:


	bool isLeaf;

	int pointerSize;

	int vanSize;
	int curStrNum;

	Node* parent;

	//operators

	bool MoveBackward(int startPos, int dist);

	int FindSplitPos(int &);

	void SetNodes(Node* firstChild, Node* secChild, char* splitString);

	void RelinkParent();
	//string comparator
	StrComparator* strcomp;

};

#endif

