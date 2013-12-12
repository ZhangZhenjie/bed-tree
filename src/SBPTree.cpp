#include "SBPTree.h"
#include "Queue.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

SBPTree::SBPTree(int pSize, BufferManager* manager, StrComparator* comp)
{

	pageSize = pSize;

	curStrNum = 0;

	queryTime = insertTime = deleteTime = 0;

	strcomp = comp;

	root = NULL;

	bm = manager;
}

SBPTree::~SBPTree()
{
	/*
	if(root)
	{
		root->ClearSubtree();
		delete root;
	}
	*/

	//delete bm;
}

bool SBPTree::InsertString(char* newString)
{
	static int curRootStrNum = 0;
	if(root == NULL)
	{
		//create the root node, with leaf node label
		root = new Node(pageSize, true, strcomp, bm);
	}

	Node* targetNode = root->FindNode(newString);
	Node* temp = targetNode->InsertNewString(newString);

	if(temp){
		root = temp;
		cout << "Node " << root->GetID() << " is new root" << endl;
		//root->PrintNode();
	}

	/*
	if(root->GetStrNum() != curRootStrNum)
	{
		//root->PrintNode();
		curRootStrNum = root->GetStrNum();
		//cout << curStrNum << endl;
	}
	*/
	//root->PrintNode();

	curStrNum++;
	insertTime++;

	//cout << curStrNum << " : " << root->CountSubtree() << " " << root->GetStrNum() << endl;
	return true;
}

void SBPTree::Verify(bool isPrint)
{
	if(root)
	{
		ofstream output("curstr.txt");
		output.close();
		char preString[2000] = "";
		//strcomp->GetMinString();
		preString[0] = -1;
		int itrNum = root->CountSubtree(preString, isPrint);

		cout << "Current String Num : " << curStrNum << endl;
		cout << "Accessible Number  : " << itrNum << endl;
	}
	else
		cout << "Empty Tree" << endl;
}

int SBPTree::TopKQuery(char* queryString, int qStrLen, int k)
{
	char* sString = strcomp->GetMinString(); 
	char* eString = strcomp->GetMaxString();

	int resSize = 0;

	strcomp->QueryInfo(queryString, qStrLen);

	Queue q(k+1);

	float threshold = 100;

	if(root)
	{
		root->TopKQuery(queryString, qStrLen, &q, k, threshold, sString, eString);
	}
	else
	{
		cout << "Emtpy Tree" << endl;
	}

	resSize = q.GetCurSize();
	void** resArray = q.GetCurArray();

	//int i;

	//cout << strcomp->PrintString(queryString) << " : " << endl;
	//for(i = 0; i < resSize; i++)
	//	cout << "  " << *((string*)resArray[i]) << endl;

	return q.GetCurSize();
}

int SBPTree::SelfJoinQuery(float threshold)
{
	char* sString = strcomp->GetMinString();
	char* eString = strcomp->GetMaxString();

	int resSize = 0;

	if(root)
	{
		resSize = Node::JoinQuery(root, root, threshold, sString, eString, sString, eString);
	}
	else
	{
		cout << "empty tree" << endl;
	}

	return resSize;
}

int SBPTree::TopKSelfJoinQuery(int k)
{
	char* sString = strcomp->GetMinString();
	char* eString = strcomp->GetMaxString();

	Queue q(k + 1);

	float threshold = 100;

	int resSize = 0;

	if(root)
	{
		Node::TopKJoinQuery(root, root, &q, k, threshold, sString, eString, sString, eString);
	}
	else
	{
		cout << "empty tree" << endl;
	}

	resSize = q.GetCurSize();

	cout << "Results : " << endl;
	for(int i = 0; i < resSize; i++)
	{
		void* delPt;
		double delDist;

		q.Dequeue(&delPt, &delDist);

		cout << i + 1 << " : " << endl;
		cout << ((SimilarPair*)delPt)->leftString << endl;
		cout << ((SimilarPair*)delPt)->rightString << endl;

		delete (SimilarPair*)delPt;
	}
	//void** resArray = q.GetCurArray();

	return resSize;
}


int SBPTree::RangeQuery(char* queryString, int qStrLen, float threshold)
{
	//cout << queryString << endl;
	//cout << "Threshold : " << threshold << endl;

	char* sString = strcomp->GetMinString(); 
	char* eString = strcomp->GetMaxString();

	int resSize = 0;

	strcomp->QueryInfo(queryString, qStrLen);

	if(root)
	{
		resSize = root->RangeQuery(queryString, qStrLen, threshold, sString, eString);

		//cout << "Result Size : " << resSize << endl;
	}
	else
	{
		cout << "Emtpy Tree" << endl;
	}

	return resSize;
}

