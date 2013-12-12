#include "Node.h"
#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

//#define _JOIN_DEBUG_

using namespace std;

Node::Node(int size, bool leaf, StrComparator* comp, BufferManager* manager) : Page(size, manager)
{
	nodeID = bm->RequestID();
	bm->AddEntry(this, nodeID);

	pointerSize = sizeof(Node*);

	isLeaf = leaf;

	vanSize = pageSize;

	curStrNum = 0;

	strcomp = comp;

	parent = NULL;

#ifdef _DEBUG_
	cout << "Node " << nodeID << " created" << endl;
#endif

}

Node::Node(int size, bool leaf, StrComparator* comp, Node* splitNode,
		   BufferManager* manager) : Page(size, manager)
{
	nodeID = bm->RequestID();
	bm->AddEntry(this, nodeID);

	pointerSize = sizeof(Node*);

	isLeaf = leaf;

	int startPos = pageSize / 2;

	vanSize = pageSize;

	curStrNum = 0;

	strcomp = comp;

	parent = NULL;

	//split operation is implemented here
	int restNum;

	int splitPos = splitNode->FindSplitPos(restNum);

	//int i;

	int copySize = pageSize - splitNode->vanSize - splitPos;

	int splitStrLen = strcomp->GetTrueSize(splitNode->buffer + splitPos);
	
	if(!isLeaf)
	{

		splitPos += splitStrLen;
		copySize -= splitStrLen;
	}

	/*
	for(i = 0; i < copySize; i++)
	{
		buffer[i] = splitNode->buffer[i+splitPos];
	}
	*/

	if(copySize > 0)
		memcpy(buffer, splitNode->buffer + splitPos, copySize);


	vanSize = pageSize - copySize;
	curStrNum = splitNode->curStrNum - restNum;

	splitNode->vanSize += copySize;
	splitNode->curStrNum = restNum;

	if(!isLeaf){
		curStrNum--;
		splitNode->vanSize += splitStrLen;
		RelinkParent();
	}

	//splitNode->PrintNode();
	//this->PrintNode();

#ifdef _DEBUG_
	cout << "Node " << nodeID << " created" << endl;
#endif

	return;
}


Node::~Node()
{

}


// insert string into leaf node
Node* Node::InsertNewString(char* newString)
{
	LoadFromFile();
	bm->WriteUpdate(nodeID);

	Node* retNode = NULL;

	if(!isLeaf)
	{
		cout << "wrong! should be leaf for a new string insertion opertaion." << endl;
		return NULL;
	}

	int offset = 0;

	//the necessary space to store the new string and the related link
	int curStrLen = strcomp->GetTrueSize(newString);
	int necSpace = curStrLen + offset;

	if(vanSize < necSpace)
	{
		//split is necessary

		Node* newNode = new Node(pageSize, isLeaf, strcomp, this, bm);

		int splitStrLen = strcomp->GetTrueSize(newNode->buffer);
		char* tempbuf = new char[splitStrLen];
		memcpy(tempbuf, newNode->buffer, splitStrLen);
		strcomp->CompressString(tempbuf);

		//if current node is previously root node
		if(!parent)
		{
			//create a new root and insert current node into root
			parent = new Node(pageSize, false, strcomp, bm);
			parent->SetNodes(this, newNode, tempbuf);

			retNode = parent;
			//parent->PrintNode();
		}
		else
		{
			retNode = parent->InsertNewEntry(newNode, tempbuf);
		}

		delete [] tempbuf;
			

		if(strcomp->isBefore(newNode->buffer, newString))
		{
			newNode->InsertNewString(newString);

			//newNode->PrintNode();

			return retNode;
		}
	}

	//the current number of string on testing
	int curPos = 0;
	//the current pointer in the buffer
	int curPt  = 0;

	while(curPos < curStrNum)
	{
		if(strcomp->isNotAfter(newString, buffer+curPt))
		{
			break;
		}

		curPos++;
		curPt += strcomp->GetTrueSize(buffer+curPt) + offset;
	}

	//insert the string here, after moving the following strings backward
	MoveBackward(curPt, necSpace);
	memcpy(buffer + curPt, newString, strcomp->GetTrueSize(newString));

	curStrNum++;
	vanSize -= necSpace;


	return retNode;
}

bool Node::MoveBackward(int startPos, int dist)
{
	bm->WriteUpdate(nodeID);

	if(startPos < 0)
		return true;

	int endPos = pageSize - vanSize - 1;

	int i;

	for(i = endPos; i >= startPos; i--)
	{
		buffer[i + dist] = buffer[i];
	}

	return true;
}

void Node::PrintNode()
{
	LoadFromFile();
	bm->ReadUpdate(nodeID);

	int curPt;
	if(isLeaf)
		curPt = 0;
	else
		curPt = pointerSize;

	int curPos = 0;

	int offset;
	if(isLeaf)
		offset = 0;
	else
		offset = pointerSize;

	cout << "Node " << nodeID << " : " << endl;
	cout << "String Size : " << curStrNum << endl;
	cout << "Vancancy Space : " << vanSize << endl;

	if(!isLeaf)
	{
		Node* childNode = *((Node**)buffer);
		cout << childNode->nodeID << endl;
	}
	while(curPos < curStrNum)
	{
		cout << buffer + curPt << endl;

		curPos++;
		curPt += strcomp->GetTrueSize(buffer + curPt) + offset;

		if(!isLeaf)
		{
			Node* childNode = *((Node**)(buffer + curPt - pointerSize));
			cout << childNode->nodeID << endl;
		}
	}

	cout << endl;
}

int Node::FindSplitPos(int &curPos)
{
	bm->ReadUpdate(nodeID);

	curPos = 0;
	int curPt;

	if(isLeaf)
		curPt = 0;
	else
		curPt = pointerSize;
	
	int midPt = pageSize / 2;

	int offset;
	if(isLeaf)
		offset = 0;
	else
		offset = pointerSize;

	while(curPos < curStrNum)
	{
		/*
		if(curPt > midPt)
			break;
		curPt += strcomp->GetTrueSize(buffer + curPt) + offset;
		curPos++;
		*/
		//a new implementation below is done on june 14, 2010

		int newPt = curPt + strcomp->GetTrueSize(buffer + curPt) + offset;;

		if(newPt > midPt)
			break;

		curPt = newPt;
		curPos++;
	}

	return curPt;
}

Node* Node::InsertNewEntry(Node* splitNode, char* insertString)
{
	LoadFromFile();
	bm->WriteUpdate(nodeID);

	//PrintNode();

	Node* retNode = NULL;

	if(isLeaf)
	{
		cout << "Mistake happens! Can't be a leaf" << endl;
		return NULL;
	}

	int offset = pointerSize;

	//char* splitString = insertString;

	int necSpace = strcomp->GetTrueSize(insertString) + offset;


	if(vanSize < necSpace)
	{
		//split is necessary
		char* splitString;

		Node* newNode = new Node(pageSize, isLeaf, strcomp, this, bm);
			
		//PrintNode();
		//newNode->PrintNode();

		//if current node is previously root node
		if(!parent)
		{
			//create a new root and insert current node into root
			parent = new Node(pageSize, false, strcomp, bm);
			
			splitString = buffer + pageSize - vanSize;

			parent->SetNodes(this, newNode, splitString);

			retNode = parent;

		}
		else
		{
			splitString = buffer + pageSize - vanSize;

			retNode = parent->InsertNewEntry(newNode, splitString);
		}

		if(strcomp->isNotAfter(splitString, insertString))
		{
			newNode->InsertNewEntry(splitNode, insertString);

			return retNode;
		}
	}

	//the current number of string on testing
	int curPos = 0;
	//the current pointer in the buffer
	int curPt  = pointerSize;

	while(curPos < curStrNum)
	{
		if(strcomp->isBefore(insertString, buffer + curPt))
		{
			break;
		}

		curPos++;
		curPt += strcomp->GetTrueSize(buffer+curPt) + offset;
	}

	MoveBackward(curPt, necSpace);
	memcpy(buffer + curPt, insertString, strcomp->GetTrueSize(insertString));
	*( (Node**)(buffer + curPt + necSpace - pointerSize) ) = splitNode;

	vanSize -= necSpace;
	curStrNum++;

	splitNode->parent = this;

#ifdef _DEBUG_
	cout << "Node " << splitNode->nodeID << " inserted into Node " << nodeID <<endl;
#endif

	//PrintNode();
	return retNode;
}

void Node::RelinkParent()
{
	LoadFromFile();
	bm->ReadUpdate(nodeID);

	int curPos, curPt;

	if(isLeaf)
	{
		cout << "mistake happens" << endl;
	}

	curPos = 0;
	curPt = pointerSize;

	Node* child;

	while(curPos <= curStrNum)
	{
		child = *((Node**)(buffer + curPt - pointerSize));

		child->parent = this;
		bm->WriteUpdate(child->nodeID);

		curPos++;
		curPt += strcomp->GetTrueSize(buffer + curPt) + pointerSize;
	}
}

void Node::ClearSubtree()
{
	LoadFromFile();
	bm->WriteUpdate(nodeID);

	int curPos, curPt;

	curPos = 0;
	curPt = pointerSize;

	Node* child;

	while(curPos <= curStrNum)
	{
		child = *((Node**)(buffer + curPt - pointerSize));

		if(child->isLeaf)
		{
			delete child;
		}
		else
			child->ClearSubtree();

		curPos++;
		curPt += strcomp->GetTrueSize(buffer + curPt) + pointerSize;
	}
}

int Node::TopKQuery(char* queryString, int qStrLen, Queue* q, int k, float& threshold,
		char* sString, char* eString)
{
	LoadFromFile();
	bm->ReadUpdate(nodeID);

	int curPos, curPt;
	int counter = 0;

	if(isLeaf){
		curPos = 0;
		curPt = 0;

		while(curPos < curStrNum)
		{
			char* tempString = buffer + curPt;

			float distance = strcomp->Dist(queryString, qStrLen, tempString, threshold);

			if( distance < threshold)
			{
				string* newres = new string(strcomp->PrintString(tempString));

				q->Enqueue(newres, (-1) * distance);

				if(q->GetCurSize() == k + 1)
				{
					void* delstr;
					double deldist;
					q->Dequeue(&delstr, &deldist);
					delete (string*)delstr;

					threshold = (-1)*q->GetTopValue();
				}
				//out << strcomp->PrintString(tempString) << endl;
			}

			curPos++;
			curPt += strcomp->GetTrueSize(tempString);
		}
	}
	else
	{
		curPos = 0;
		curPt = pointerSize;

		Node* child;
		
		/*
		char* tStartString = new char[400];
		char* tEndString = new char[400];
		char* tPointer;
		*/
		char* tStartString;
		char* tEndString;
		int tEndPos = 0;

		while(curPos <= curStrNum)
		{
			child = *((Node**)(buffer + curPt - pointerSize));

			LoadFromFile();
			bm->ReadUpdate(nodeID);

			if(curPos == 0)
			{
				tStartString = sString;
			}
			else
			{
				tStartString = buffer + tEndPos;
			}
			
			if(curPos == curStrNum)
			{
				tEndString = eString;
			}
			else
			{
				tEndPos = curPt;
				tEndString = buffer + tEndPos;
			}
		
			float lb;

			lb = strcomp->LowerBound(queryString, qStrLen, tStartString, 
					tEndString, threshold);

			if(lb <= threshold)
			{
				counter += child->TopKQuery(queryString, qStrLen, q, k, threshold,
					tStartString, tEndString);
			}

			LoadFromFile();

			curPos++;
			curPt += strcomp->GetTrueSize(buffer + curPt) + pointerSize;
		}

		//delete [] tStartString;
		//delete [] tEndString;
	}

	return counter;
}


int Node::RangeQuery(char* queryString, int qStrLen, float threshold, 
					 char* sString, char* eString)
{
	LoadFromFile();
	bm->ReadUpdate(nodeID);

	int curPos, curPt;

	int counter = 0;

	if(isLeaf){
		curPos = 0;
		curPt = 0;

		while(curPos < curStrNum)
		{
			char* tempString = buffer + curPt;

			float distance = strcomp->Dist(queryString, qStrLen, tempString, threshold);

			if( distance <= threshold)
			{
				//cout << strcomp->PrintString(tempString) << endl;
				counter++;
			}

			curPos++;
			curPt += strcomp->GetTrueSize(tempString);
		}
	}
	else
	{
		curPos = 0;
		curPt = pointerSize;

		Node* child;
		
		char* tStartString;
		char* tEndString;
		int tEndPos = 0;

		while(curPos <= curStrNum)
		{
			child = *((Node**)(buffer + curPt - pointerSize));

			LoadFromFile();
			bm->ReadUpdate(nodeID);

			if(curPos == 0)
				tStartString = sString;
			else
				tStartString = buffer + tEndPos;
			
			if(curPos == curStrNum)
				tEndString = eString;
			else
			{
				tEndPos = curPt;
				tEndString = buffer + tEndPos;
			}
		
			float lb;

			lb = strcomp->LowerBound(queryString, qStrLen, tStartString, 
					tEndString, threshold);

			if(lb <= threshold){
				counter += child->RangeQuery(queryString, qStrLen, threshold,
					tStartString, tEndString);
			}

			LoadFromFile();

			curPos++;
			curPt += strcomp->GetTrueSize(buffer + curPt) + pointerSize;
		}

	}

	return counter;
}

int Node::CountSubtree(char* preString, bool isPrint)
{
	//cout << "counting node " << nodeID << endl;
	LoadFromFile();
	bm->ReadUpdate(nodeID);

	int curPos, curPt;

	ofstream output;

	if(isLeaf){
		
		if(isPrint)
			output.open("curstr.txt", ios::app);

		curPos = 0;
		curPt = 0;

		
		while(curPos < curStrNum)
		{

			if(isPrint)
			{
				output << buffer + curPt << endl;
			}

/*
			if(isPrint)
			{
				if(!strcomp->isEqual(preString, buffer + curPt)){
					output << buffer + curPt << endl;
                }
			}
			
			if(preString[0] != -1)
			{
				if(!strcomp->isNotAfter(preString, buffer + curPt))
				{
					cout << preString << endl;
					cout << buffer + curPt << endl;
					cout << endl;
				}
			}

			memcpy(preString, buffer + curPt, strcomp->GetTrueSize(buffer + curPt));
*/			
			curPos++;
			curPt += strcomp->GetTrueSize(buffer + curPt);
		}

		if(isPrint)
			output.close();

		return curStrNum;
	}

	curPos = 0;
	curPt = pointerSize;

	int sum = 0;

	Node* child;

	while(curPos <= curStrNum)
	{
		child = *((Node**)(buffer + curPt - pointerSize));

		sum += child->CountSubtree(preString, isPrint);

		LoadFromFile();

		curPos++;
		curPt += strcomp->GetTrueSize(buffer + curPt) + pointerSize;
	}

	return sum;
}


Node* Node::FindNode(char* queryString)
{
	LoadFromFile();
	bm->ReadUpdate(nodeID);

	int offset;

	int curPos = 0;
	int curPt = 0;

	if(isLeaf)
	{
		return this;
	}
	else
	{
		offset = pointerSize;

		curPt = pointerSize;

		char* tempstr;
		Node* tempNode;

		while(curPos < curStrNum)
		{
			tempstr = buffer + curPt;

			if(strcomp->isBefore(queryString, tempstr))
			{
				tempNode = *((Node**)(buffer + curPt - pointerSize));

				return tempNode->FindNode(queryString);
			}

			curPos++;
			curPt += strcomp->GetTrueSize(tempstr) + offset;
		}

		tempNode = *((Node**)(buffer + curPt - pointerSize));

		return tempNode->FindNode(queryString);
	}
}

void Node::SetNodes(Node* firstChild, Node* secChild, char* splitString)
{
	LoadFromFile();
	bm->WriteUpdate(nodeID);

	int curPt = 0;

	void** pt = (void**)(buffer + curPt);

	*pt = firstChild;

	curPt += pointerSize;

	memcpy(buffer + curPt, splitString, strcomp->GetTrueSize(splitString));

	int splitStrSize = strcomp->GetTrueSize(splitString);

	curPt += splitStrSize;

	*((Node**)(buffer + curPt)) = secChild;

	curStrNum = 1;
	vanSize = pageSize - splitStrSize - 2 * pointerSize;

	firstChild->parent = this;
	secChild->parent = this;

	bm->WriteUpdate(firstChild->nodeID);
	bm->WriteUpdate(secChild->nodeID);

#ifdef _DEBUG_
	cout << "Node " << firstChild->nodeID << " inserted into Node " << nodeID << endl;
	cout << "Node " << secChild->nodeID << " inserted into Node " << nodeID << endl;
#endif

}

int Node::JoinQuery(Node* lNode, Node* rNode, float threshold, 
					char* lsString, char* leString, char* rsString, char* reString)
{
	int counter = 0;
	int pSize = sizeof(Node*);

	lNode->LoadFromFile();
	lNode->bm->ReadUpdate(lNode->nodeID);

	rNode->LoadFromFile();
	rNode->bm->ReadUpdate(rNode->nodeID);

	if(lNode->isLeaf)
	{
		if(!rNode->isLeaf)
		{
			cout << "both pages must be leaves." << endl;
			return 0;
		}

		//cout << lNode->nodeID << " " << rNode->nodeID << endl;
		int lCurPos = 0;
		int lCurPt = 0;

		while(lCurPos < lNode->curStrNum)
		{
			char* lString = lNode->buffer + lCurPt;

			int rCurPos = 0;
			int rCurPt = 0;

			while(rCurPos < rNode->curStrNum)
			{
				char* rString = rNode->buffer + rCurPt;

				if(lString != rString)
				{
					float distance = lNode->strcomp->Dist(lString, rString, threshold);

					if( distance <= threshold)
					{
#ifdef _JOIN_DEBUG_
						cout << lNode->strcomp->PrintString(lString) << "\t";
						cout << rNode->strcomp->PrintString(rString) << endl;
#endif
						counter++;
					}
				}
				rCurPos++;
				rCurPt += rNode->strcomp->GetTrueSize(rString);
			}

			lCurPos++;
			lCurPt += lNode->strcomp->GetTrueSize(lString);
		}
	}
	else
	{
		int lCurPos = 0;
		int lCurPt = pSize;

		Node* lChild;
		
		char* lStartString;
		char* lEndString;
		int lEndPos = 0;
		
		while(lCurPos <= lNode->curStrNum)
		{
			lChild = *((Node**)(lNode->buffer + lCurPt - pSize));

			if(lCurPos == 0)
				lStartString = lsString;
			else
				lStartString = lNode->buffer + lEndPos;
			
			if(lCurPos == lNode->curStrNum)
				lEndString = leString;
			else
			{
				lEndPos = lCurPt;
				lEndString = lNode->buffer + lEndPos;
			}

			int rCurPos, rCurPt, rEndPos;

			if(lNode->nodeID == rNode->nodeID)
			{
				rCurPos = lCurPos;
				rCurPt = lCurPt;
				rEndPos = lEndPos;
			}
			else
			{
				rCurPos = 0;
				rCurPt = pSize;
				rEndPos = 0;
			}

			Node* rChild;
		
			char* rStartString;
			char* rEndString;

			while(rCurPos <= rNode->curStrNum)
			{
				rChild = *((Node**)(rNode->buffer + rCurPt - pSize));

				if(rCurPos == 0)
					rStartString = rsString;
				else
					rStartString = rNode->buffer + rEndPos;

				if(rCurPos == rNode->curStrNum)
					rEndString = reString;
				else
				{
					rEndPos = rCurPt;
					rEndString = rNode->buffer + rEndPos;
				}

				//if(lNode->nodeID <= rNode->nodeID)
				//{
				float lb = 0;
#ifdef _JOIN_DEBUG_
				cout << lNode->strcomp->PrintString(lStartString) << endl;
				cout << lNode->strcomp->PrintString(lEndString) << endl;
				cout << rNode->strcomp->PrintString(rStartString) << endl;
				cout << rNode->strcomp->PrintString(rEndString) << endl;
#endif
				if(lNode->nodeID != rNode->nodeID)
					lb = lNode->strcomp->LowerBound(lStartString, lEndString, rStartString, rEndString, threshold);
#ifdef _JOIN_DEBUG_
				cout << lb << endl;
				cout << endl;
#endif

				if(lb <= threshold){
					int resSize = JoinQuery(lChild, rChild, threshold, lStartString, lEndString,
						rStartString, rEndString);

					counter += resSize;
				}
				//}

				rCurPos++;
				rCurPt += rNode->strcomp->GetTrueSize(rNode->buffer + rCurPt) + pSize;
			}

			lCurPos++;
			lCurPt += lNode->strcomp->GetTrueSize(lNode->buffer + lCurPt) + pSize;
		}

	}
	
	return counter;
}

int Node::TopKJoinQuery(Node* lNode, Node* rNode, Queue* q, int k, float& threshold, 
					char* lsString, char* leString, char* rsString, char* reString)
{
	//int counter = 0;
	int pSize = sizeof(Node*);

	lNode->LoadFromFile();
	lNode->bm->ReadUpdate(lNode->nodeID);

	rNode->LoadFromFile();
	rNode->bm->ReadUpdate(rNode->nodeID);

	if(lNode->isLeaf)
	{
		if(!rNode->isLeaf)
		{
			cout << "both pages must be leaves." << endl;
			return 0;
		}

		//cout << lNode->nodeID << " " << rNode->nodeID << endl;
		int lCurPos = 0;
		int lCurPt = 0;

		while(lCurPos < lNode->curStrNum)
		{
			char* lString = lNode->buffer + lCurPt;

			int rCurPos = 0;
			int rCurPt = 0;

			while(rCurPos < rNode->curStrNum)
			{
				char* rString = rNode->buffer + rCurPt;

				if(lString != rString)
				{
					float distance = lNode->strcomp->Dist(lString, rString, threshold);

					if( distance <= threshold)
					{
						char* newLStr = lNode->strcomp->RemoveHead(lString);
						char* newRStr = lNode->strcomp->RemoveHead(rString);

						SimilarPair* sp = new SimilarPair(newLStr, newRStr);

						q->Enqueue(sp, (-1) * distance);

						if(q->GetCurSize() == k + 1)
						{
							void* delPt;
							double delDist;

							q->Dequeue(&delPt, &delDist);

							delete (SimilarPair*)(delPt);

							threshold = (-1)*q->GetTopValue() - 1;
						}
					}
				}
				rCurPos++;
				rCurPt += rNode->strcomp->GetTrueSize(rString);
			}

			lCurPos++;
			lCurPt += lNode->strcomp->GetTrueSize(lString);
		}
	}
	else
	{
		int lCurPos = 0;
		int lCurPt = pSize;

		Node* lChild;
		
		char* lStartString;
		char* lEndString;
		int lEndPos = 0;
		
		while(lCurPos <= lNode->curStrNum)
		{
			lChild = *((Node**)(lNode->buffer + lCurPt - pSize));

			if(lCurPos == 0)
				lStartString = lsString;
			else
				lStartString = lNode->buffer + lEndPos;
			
			if(lCurPos == lNode->curStrNum)
				lEndString = leString;
			else
			{
				lEndPos = lCurPt;
				lEndString = lNode->buffer + lEndPos;
			}

			int rCurPos, rCurPt, rEndPos;

			if(lNode->nodeID == rNode->nodeID)
			{
				rCurPos = lCurPos;
				rCurPt = lCurPt;
				rEndPos = lEndPos;
			}
			else
			{
				rCurPos = 0;
				rCurPt = pSize;
				rEndPos = 0;
			}

			Node* rChild;
		
			char* rStartString;
			char* rEndString;

			while(rCurPos <= rNode->curStrNum)
			{
				rChild = *((Node**)(rNode->buffer + rCurPt - pSize));

				if(rCurPos == 0)
					rStartString = rsString;
				else
					rStartString = rNode->buffer + rEndPos;

				if(rCurPos == rNode->curStrNum)
					rEndString = reString;
				else
				{
					rEndPos = rCurPt;
					rEndString = rNode->buffer + rEndPos;
				}

				//if(lNode->nodeID <= rNode->nodeID)
				//{
				float lb = 0;
#ifdef _JOIN_DEBUG_
				cout << lNode->strcomp->PrintString(lStartString) << endl;
				cout << lNode->strcomp->PrintString(lEndString) << endl;
				cout << rNode->strcomp->PrintString(rStartString) << endl;
				cout << rNode->strcomp->PrintString(rEndString) << endl;
#endif
				if(lNode->nodeID != rNode->nodeID)
					lb = lNode->strcomp->LowerBound(lStartString, lEndString, rStartString, rEndString, threshold);
#ifdef _JOIN_DEBUG_
				cout << lb << endl;
				cout << endl;
#endif

				if(lb <= threshold){
					int resSize = TopKJoinQuery(lChild, rChild, q, k, threshold, lStartString, lEndString,
						rStartString, rEndString);

					//counter += resSize;
				}
				//}

				rCurPos++;
				rCurPt += rNode->strcomp->GetTrueSize(rNode->buffer + rCurPt) + pSize;
			}

			lCurPos++;
			lCurPt += lNode->strcomp->GetTrueSize(lNode->buffer + lCurPt) + pSize;
		}

	}
	
	//return counter;

	return q->GetCurSize();
}

SimilarPair::SimilarPair(char* lstr, char* rstr)
{
	leftString = lstr; 
	rightString = rstr;

	/*
	string t = leftString;
	t += " : ";
	t += rightString;

	cout << t << endl; 
	*/
}

SimilarPair::~SimilarPair()
{
}
