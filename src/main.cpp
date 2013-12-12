#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <cstdlib>

#include "NewRand.h"
#include "SBPTree.h"
#include "StrDictOrder.h"
#include "BufferManager.h"
#include "GramDictOrder.h"
#include "GramLocOrder.h"
#include "NormGramDictOrder.h"
#include "InvertedList.h"

using namespace std;

#define MAXLEN 2000
#define KB 1024

void NameQuery(int dist)
{
	int i;

	char buffer[512];

	ifstream input("query-name.txt");

	ofstream output("query-name-new.txt");

	for(i = 0; i < 1418; i++)
	{
		input.getline(buffer, 512);

		output << i + 1 << " " << buffer << endl;
		output << dist << endl;
	}

	output.close();
}

void GenQuery(char* inputfile, char* queryFile, int lineNum, int queryNum,
			  int maxDist)
{
	int i;

	ifstream input(inputfile);
	ofstream output(queryFile);

	char** queryArray = new char*[queryNum];

	char buffer[MAXLEN];

	for(i = 0; i < queryNum; i++)
		queryArray[i] = new char[MAXLEN];

	for(i = 0; i < queryNum; i++)
	{
		input.getline(buffer, MAXLEN);

		strcpy(queryArray[i], buffer);
	}

	for(i = queryNum; i < lineNum; i++)
	{
		input.getline(buffer, MAXLEN);

		int randNum = ( ( (rand() % (1 << 16)) << 16 ) + rand() % (1 << 16));
		int randPos = randNum % i;

		if(randPos < queryNum)
		{
			//cout << "String " << i << " replacing String " << randPos << endl;
			strcpy(queryArray[randPos], buffer);
		}
	}

	for(i = 0; i < queryNum; i++)
	{
		int pos = 0;

		while(*(queryArray[i] + pos) != ' ')
			pos++;

		while(*(queryArray[i] + pos) == ' ')
			pos++;

		output << queryArray[i] + pos << endl;

		//int randDist = rand() % maxDist + 1;
		int randDist = maxDist;

		//output << randDist << endl;
	}


	for(i = 0; i < queryNum; i++)
		delete [] queryArray[i];
	delete [] queryArray;

	input.close();
	output.close();
}

void LinearScanMethod(StrComparator* strcomp, char* filename, int strNum,
					  char* queryfile, int queryNum)
{
	ifstream input(queryfile);
	ifstream strfile;

	int startTime = time(NULL);

	char orgbuf[MAXLEN];
	char buffer[MAXLEN];
	char orgquerybuf[MAXLEN];
	char queryString[MAXLEN];
	char distString[10];

	int i,j;

	//int distArray[MAXLEN];
	//for(i = 0; i < MAXLEN; i++)
	//{
	//	distArray[i] = 0;
	//}

	//int k = 3;

	int totalResSize = 0;
	float threshold;

	for(i = 0; i < queryNum; i++)
	{
		int curSize = 0;

		strfile.open(filename);

		input.getline(orgquerybuf, MAXLEN);
		strcomp->OrgProcessing(orgquerybuf, queryString);
		/*
		queryString = orgquerybuf;
		while(*queryString != ' ')
			queryString++;
		while(*queryString == ' ')
			queryString++;
		*/

		//cout << i + 1 << endl;
		//cout << strcomp->PrintString(queryString) << endl;

		//if(i < 8)
		//	continue;
		input.getline(distString, 10);
		threshold = atof(distString);
		//strcomp->QueryInfo(queryString, strlen(queryString));
		//Queue q(k+1);
		//threshold = MAXLEN;

		for(j = 0; j < strNum; j++)
		{
			//if(j % 1000 == 0)
			//	cout << j << endl;
			strfile.getline(orgbuf, MAXLEN);
			strcomp->OrgProcessing(orgbuf, buffer);
			/*
			buffer = orgbuf;
			if(*buffer != '\0')
			{
				while(*buffer != ' ')
					buffer++;
				while(*buffer == ' ')
					buffer++;
			}
			*/
			//int stringlen = strlen(buffer);

			//cout << strcomp->PrintString(buffer) << endl;
			//cout << strcomp->PrintString(queryString) << endl;
			float distance = strcomp->Dist(buffer, queryString, threshold);
			//cout << distance << endl;
			//distArray[distance]++;

			if(distance <= threshold)
			{
				//cout << j + 1 << endl;
				//cout << strcomp->PrintString(buffer) << endl;
				//cout << distance << endl;
				//string* newres = new string(buffer);
				//q.Enqueue(newres, (-1)*distance);

				//if(q.GetCurSize() == k+1)
				//{
				//	void* delstr;
				//	double dist;
				//	q.Dequeue(&delstr,&dist);
				//	delete ((string*)delstr);
				//	threshold = (-1)*q.GetTopValue();
				//}
				//cout << buffer << endl;
				curSize++;
				totalResSize++;
			}
		}
		//int topkSize = q.GetCurSize();
		//void** resArray = q.GetCurArray();
		//cout << queryString << endl;
		//for(j = 0; j < topkSize; j++)
		//{
		//	cout << "  " << *((string*)resArray[j]) << endl;
			//char* p = ((string*)resArray[j])->c_str();
			//cout << strcomp->Dist(queryString, p, threshold) << endl;
		//	delete ((string*)resArray[j]);
		//}
		//cout << endl;
		//cout << "Query " << i + 1 << " : " << curSize << endl;
		strfile.close();
	}
	//int maxDist = MAXLEN - 1;

	//while(maxDist >= 0)
	//{
	//	if(distArray[maxDist] > 0)
	//		break;
	//	maxDist--;
	//}

	//ofstream distFile("dist.txt");
	//for(i = 0; i <= maxDist; i++)
	//{
	//	distFile << i << " " << distArray[i] << endl;
	//}

	cout << "Total Res Size : " << totalResSize << endl;
	cout << "Querying time : " << time(NULL) - startTime << endl;
	cout << endl;
}

void TopKLinearScanMethod(StrComparator* strcomp, char* filename, int strNum,
					  char* queryfile, int queryNum)
{
	ifstream input(queryfile);
	ifstream strfile;

	int startTime = time(NULL);

	char orgbuf[MAXLEN];
	char buffer[MAXLEN];
	char orgquerybuf[MAXLEN];
	char queryString[MAXLEN];
	char distString[10];

	int i,j;

	int distArray[MAXLEN];
	for(i = 0; i < MAXLEN; i++)
	{
		distArray[i] = 0;
	}

	int k;

	int totalResSize = 0;
	float threshold;
	for(i = 0; i < queryNum; i++)
	{
		int curSize = 0;

		strfile.open(filename);

		input.getline(orgquerybuf, MAXLEN);
		strcomp->PreProcessing(orgquerybuf, queryString);
		input.getline(distString, 10);

		//cout << queryString << " " << distString << endl;

		k = atoi(distString);
		Queue q(k+1);
		threshold = MAXLEN;

		for(j = 0; j < strNum; j++)
		{
			//if(j % 1000 == 0)
			//	cout << j << endl;
			strfile.getline(orgbuf, MAXLEN);
			strcomp->PreProcessing(orgbuf, buffer);

			float distance = strcomp->Dist(buffer, queryString, threshold);
			//distArray[distance]++;

			if(distance <= threshold)
			{
				//cout << buffer << endl;
				string* newres = new string(buffer);
				q.Enqueue(newres, (-1)*distance);

				if(q.GetCurSize() == k+1)
				{
					void* delstr;
					double dist;
					q.Dequeue(&delstr,&dist);
					delete ((string*)delstr);
					threshold = (-1)*q.GetTopValue();
				}
				curSize++;
				totalResSize++;
			}
		}

		int topkSize = q.GetCurSize();
		void** resArray = q.GetCurArray();
		cout << queryString << endl;
		for(j = 0; j < topkSize; j++)
		{
			cout << "  " << *((string*)resArray[j]) << endl;
			//char* p = ((string*)resArray[j])->c_str();
			//cout << strcomp->Dist(queryString, p, threshold) << endl;
			delete ((string*)resArray[j]);
		}
		cout << endl;
		//cout << "Query " << i + 1 << " : " << curSize << endl;
		strfile.close();
	}
	int maxDist = MAXLEN - 1;

	while(maxDist >= 0)
	{
		if(distArray[maxDist] > 0)
			break;
		maxDist--;
	}

	ofstream distFile("dist.txt");
	for(i = 0; i <= maxDist; i++)
	{
		distFile << i << " " << distArray[i] << endl;
	}

	cout << "Total Res Size : " << totalResSize << endl;
	cout << "Querying time : " << time(NULL) - startTime << endl;
	cout << endl;
}

void InvertedListMethod(int maxNodeNum, int bufferSize, int pageSize, StrComparator* strcomp,
						char* filename, int strNum, int gSize, int maxDist, 
						BufferManager* bm, char* queryfile, int queryNum)
{
	ifstream input(queryfile);

	//BufferManager* bm = new BufferManager(maxNodeNum, bufferSize / pageSize, pageSize);	

	InvertedList list(pageSize * KB, gSize, MAXLEN, strcomp, bm);

	int startTime = time(NULL);
		
	list.ConstructCount(filename, strNum);

	list.ConstructIndex(filename, strNum, maxDist);

	int curIOTime = bm->GetIOTime();

	cout << "Number of nodes : " << bm->GetCurSize() << endl;
	cout << "Total I/O time : " << curIOTime << endl;
	cout << "CPU time : " << time(NULL) - startTime << endl;


	startTime = time(NULL);
	int startClock = clock();
	char buffer[MAXLEN];
	char queryString[MAXLEN];
	char distString[10];
	/*
	char queryString[] = 
		"25464 AND and RO parallelism in dprolog (abstract).";
	strcomp->OrgProcessing(queryString, buffer);
	char* q = strcomp->RemoveHead(buffer);

	list.RangeQuery(q, 2);
	cout << "Querying I/O time : " << bm->GetIOTime() - curIOTime << endl;
	*/
	int totalResSize = 0;
	int i;

	for(i = 0; i < queryNum; i++)
	{
		input.getline(queryString, MAXLEN);
		input.getline(distString, 10);

		strcomp->OrgProcessing(queryString, buffer);

		totalResSize += list.RangeQuery(buffer, atoi(distString));
	}

	cout << "Total Result Size : " << totalResSize << endl;
	cout << "Querying I/O time : " << bm->GetIOTime() - curIOTime << endl;

	int queryTime = time(NULL) - startTime;
	double clockTime = (clock() - (double)startClock)/CLOCKS_PER_SEC;

	if(queryTime > 5)
		cout << "Querying time : " << queryTime << endl;
	else
		cout << "Querying time : " << clockTime << endl;

	cout << "Dist Computation : " << strcomp->GetDistCompTime() << endl;
	cout << "LB Computation : " << strcomp->GetLBCompTime() << endl;

	cout << endl;
	//delete bm;

}

void TopKTreeMethod(int maxNodeNum, int bufferSize, int pageSize, StrComparator* strcomp,
				char* filename, int strNum, BufferManager* bm, char* queryfile,
				int queryNum)
{
	int i;

	SBPTree tree( pageSize * KB, bm, strcomp); 

	ifstream inputFile(filename);

	//int id;
	string s;

	char buffer[MAXLEN + 1];
	char buffer2[MAXLEN + 1];

	int startTime = time(NULL);
	///////////////////////////////////////////////////////////
	//struct timeval tim;
    //gettimeofday(&tim, NULL);
    //double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
	//
	// time measuring with gettimeofday()
	///////////////////////////////////////////////////////////

    cout << "testing" << endl;
    
	for(i = 0; i < strNum; i++)
	{
		//cout << i << endl;
		//read the string from the file
		inputFile.getline(buffer, MAXLEN);

        char* tempStr = buffer;
        while(*tempStr != ' ')
                tempStr++;
        while(*tempStr == ' ')
                tempStr++;
        if(strlen(tempStr) <= 3)
			continue;
		strcomp->OrgProcessing(buffer, buffer2);

		tree.InsertString(buffer2);
	}

	int curIOTime = bm->GetIOTime();


	cout << "Number of nodes   : " << bm->GetCurSize() << endl;
	cout << "Building I/O time : " << curIOTime << endl;

	///////////////////////////////////////////////////////////////////////////
	//gettimeofday(&tim, NULL);
    //double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
	//cout << "Building CPU time : " << t2 - t1 << " seconds" << endl;
	cout << "Building CPU time : " << time(NULL) - startTime << " seconds" << endl;
	///////////////////////////////////////////////////////////////////////////

	//tree.Verify(true);

	///////////////////////////////////////////////////////////////////////////
	startTime = time(NULL);
	int startClock = clock();
	//gettimeofday(&tim, NULL);
    //t1=tim.tv_sec+(tim.tv_usec/1000000.0);
	///////////////////////////////////////////////////////////////////////////

	char queryString[MAXLEN];
	char kString[10];

	ifstream input(queryfile);

	int totalResSize = 0;

	for(i = 0; i < queryNum; i++)
	{
		input.getline(queryString, MAXLEN);
		//cout << queryString << endl;
		input.getline(kString, 10);
		//cout << kString << endl;

		int qStrLen = strcomp->OrgProcessing(queryString, buffer);

		cout << "Query " << i + 1 << " : " << queryString << endl;

		int k = atoi(kString);
		//int k = 3;

		int s = tree.TopKQuery(buffer, qStrLen, k);

		//cout << s << endl;
		totalResSize += s;
	}

	cout << "Total Result Size : " << totalResSize << endl;
	cout << "Querying I/O time : " << bm->GetIOTime() - curIOTime << endl;


	
	////////////////////////////////////////////////////////////////////////
	int queryTime = time(NULL) - startTime;
	double clockTime = (clock() - (double)startClock)/CLOCKS_PER_SEC;

	if(queryTime > 5)
		cout << "Querying CPU time : " << queryTime << endl;
	else
		cout << "Querying time     : " << clockTime << " seconds" << endl;
	
	//gettimeofday(&tim, NULL);
    //t2=tim.tv_sec+(tim.tv_usec/1000000.0);
	//cout << "Querying CPU time : " << t2 - t1 << endl;
	//////////////////////////////////////////////////////////////////////////


	cout <<     "Candidate Num     : " << strcomp->GetDistCompTime() << endl;
	cout <<     "Nodes Tested      : " << strcomp->GetLBCompTime() << endl;
	

	inputFile.close();

	//delete strcomp;
	//delete bm;

	cout << endl;
}

void JoinTreeMethod(int maxNodeNum, int bufferSize, int pageSize, StrComparator* strcomp,
				char* filename, int strNum, BufferManager* bm, float threshold)
{
	int i;

	SBPTree tree( pageSize * KB, bm, strcomp); 

	ifstream inputFile(filename);

	//int id;
	string s;

	char buffer[MAXLEN + 1];
	char buffer2[MAXLEN + 1];

	int startTime = time(NULL);

	for(i = 0; i < strNum; i++)
	{
		inputFile.getline(buffer, MAXLEN);

        char* tempStr = buffer;
        while(*tempStr != ' ')
                tempStr++;
        while(*tempStr == ' ')
                tempStr++;
        if(strlen(tempStr) <= 3)
			continue;

		strcomp->OrgProcessing(buffer, buffer2);

		/*
		int isDuplicate = tree.RangeQuery(buffer2, strlen(buffer2), 0);

		if(isDuplicate == 0)
		{
			tree.InsertString(buffer2);
		}
		*/
		tree.InsertString(buffer2);

	}

	int curIOTime = bm->GetIOTime();


	cout << "Number of nodes   : " << bm->GetCurSize() << endl;
	cout << "Building I/O time : " << curIOTime << endl;

	cout << "Building CPU time : " << time(NULL) - startTime << " seconds" << endl;

	//tree.Verify(true);

	///////////////////////////////////////////////////////////////////////////
	startTime = time(NULL);
	int startClock = clock();
	///////////////////////////////////////////////////////////////////////////

	int totalResSize = 0;

	totalResSize = tree.SelfJoinQuery(threshold);

	cout << "Total Result Size : " << totalResSize << endl;
	cout << "Querying I/O time : " << bm->GetIOTime() - curIOTime << endl;


	////////////////////////////////////////////////////////////////////////
	int queryTime = time(NULL) - startTime;
	double clockTime = (clock() - (double)startClock)/CLOCKS_PER_SEC;

	if(queryTime > 5)
		cout << "Querying CPU time : " << queryTime << endl;
	else
		cout << "Querying time     : " << clockTime << " seconds" << endl;
	
	//gettimeofday(&tim, NULL);
    //t2=tim.tv_sec+(tim.tv_usec/1000000.0);
	//cout << "Querying CPU time : " << t2 - t1 << endl;
	//////////////////////////////////////////////////////////////////////////


	cout <<     "Candidate Num     : " << strcomp->GetDistCompTime() << endl;
	cout <<     "Nodes Tested      : " << strcomp->GetLBCompTime() << endl;
	

	inputFile.close();

	//delete strcomp;
	//delete bm;

	cout << endl;
}

void TopKJoinTreeMethod(int maxNodeNum, int bufferSize, int pageSize, StrComparator* strcomp,
				char* filename, int strNum, BufferManager* bm, int k)
{
	int i;

	SBPTree tree( pageSize * KB, bm, strcomp); 

	ifstream inputFile(filename);

	//int id;
	string s;

	char buffer[MAXLEN + 1];
	char buffer2[MAXLEN + 1];

	int startTime = time(NULL);

	for(i = 0; i < strNum; i++)
	{
		inputFile.getline(buffer, MAXLEN);

		//cout << i + 1 << " : " << buffer << endl;

        char* tempStr = buffer;
        while(*tempStr != ' ')
                tempStr++;
        while(*tempStr == ' ')
                tempStr++;
        if(strlen(tempStr) <= 3)
			continue;

		strcomp->OrgProcessing(buffer, buffer2);

		int isDuplicate = tree.RangeQuery(buffer2, strlen(buffer2), 0);

		if(isDuplicate == 0)
		{
			tree.InsertString(buffer2);
		}
	}

	int curIOTime = bm->GetIOTime();


	cout << "Number of nodes   : " << bm->GetCurSize() << endl;
	cout << "Building I/O time : " << curIOTime << endl;

	cout << "Building CPU time : " << time(NULL) - startTime << " seconds" << endl;

	//tree.Verify(true);

	///////////////////////////////////////////////////////////////////////////
	startTime = time(NULL);
	int startClock = clock();
	///////////////////////////////////////////////////////////////////////////

	int totalResSize = 0;

	totalResSize = tree.TopKSelfJoinQuery(k);

	cout << "Total Result Size : " << totalResSize << endl;
	cout << "Querying I/O time : " << bm->GetIOTime() - curIOTime << endl;


	////////////////////////////////////////////////////////////////////////
	int queryTime = time(NULL) - startTime;
	double clockTime = (clock() - (double)startClock)/CLOCKS_PER_SEC;

	if(queryTime > 5)
		cout << "Querying CPU time : " << queryTime << endl;
	else
		cout << "Querying time     : " << clockTime << " seconds" << endl;
	
	//gettimeofday(&tim, NULL);
    //t2=tim.tv_sec+(tim.tv_usec/1000000.0);
	//cout << "Querying CPU time : " << t2 - t1 << endl;
	//////////////////////////////////////////////////////////////////////////


	cout <<     "Candidate Num     : " << strcomp->GetDistCompTime() << endl;
	cout <<     "Nodes Tested      : " << strcomp->GetLBCompTime() << endl;
	

	inputFile.close();

	//delete strcomp;
	//delete bm;

	cout << endl;
}

void TreeMethod(int maxNodeNum, int bufferSize, int pageSize, StrComparator* strcomp,
				char* filename, int strNum, BufferManager* bm, char* queryfile,
				int queryNum)
{
	int i;

	SBPTree tree( pageSize * KB, bm, strcomp); 

	ifstream inputFile(filename);

	//int id;
	string s;

	char buffer[MAXLEN + 1];
	char buffer2[MAXLEN + 1];

	int startTime = time(NULL);

	for(i = 0; i < strNum; i++)
	{
		//cout << i << endl;
		//read the string from the file
		inputFile.getline(buffer, MAXLEN);

        char* tempStr = buffer;
        while(*tempStr != ' ')
                tempStr++;
        while(*tempStr == ' ')
                tempStr++;
        if(strlen(tempStr) <= 3)
			continue;
		//if(i == 35441)
		//	i = i;
		strcomp->OrgProcessing(buffer, buffer2);

		tree.InsertString(buffer2);

		
		//if(i == 91448)
		//{
		//	cout << i << endl;
		//	tree.Verify(true);
		//}
		
	}

	int curIOTime = bm->GetIOTime();

	cout << "Number of nodes   : " << bm->GetCurSize() << endl;
	cout << "Building I/O time : " << curIOTime << endl;
	cout << "Building CPU time : " << time(NULL) - startTime << " seconds" << endl;

	//tree.Verify(true);

	//return;

	int totalClockCycles = 0;

	startTime = time(NULL);
	int startClock;

	char queryString[MAXLEN];
	char distString[10];

	ifstream input(queryfile);

	int totalResSize = 0;

	for(i = 0; i < queryNum; i++)
	{
		input.getline(queryString, MAXLEN);
		input.getline(distString, 10);

		int qStrLen = strcomp->OrgProcessing(queryString, buffer);


		startClock = clock();

		float threshold = atof(distString);

		int s = tree.RangeQuery(buffer, qStrLen, threshold);

		totalClockCycles += clock() - startClock;
		//cout << s << endl;
		
		//cout << "Query " << i + 1 << " : " << s << endl;

		totalResSize += s;
	}

	cout << "Total Result Size : " << totalResSize << endl;
	cout << "Querying I/O time : " << bm->GetIOTime() - curIOTime << endl;


	int queryTime = time(NULL) - startTime;
	double clockTime = ((double)totalClockCycles) / CLOCKS_PER_SEC;
	//double clockTime = (clock() - (double)startClock)/CLOCKS_PER_SEC;

	if(queryTime > 5)
		cout << "Querying CPU time : " << queryTime << endl;
	else
		cout << "Querying time     : " << clockTime << " seconds" << endl;
	cout <<     "Candidate Num     : " << strcomp->GetDistCompTime() << endl;
	cout <<     "Nodes Tested      : " << strcomp->GetLBCompTime() << endl;
	

	inputFile.close();

	//delete strcomp;
	//delete bm;

	cout << endl;
}

int main(int argc, char* argv[])
{
	char* queryPar = argv[1];
	char* distPar = argv[2];
	char* filename = argv[3];
	int strNum = atoi(argv[4]);
	int pageSize = atoi(argv[5]);
	int bufferSize = atoi(argv[6]);
	int maxNodeNum = atoi(argv[7]);
	char* queryfile = argv[8];
	int queryNum = atoi(argv[9]);

	//GenQuery(filename, "query-dblp.txt", strNum, 100, 16);

	cout << "String File Name : " << filename << endl;
	cout << "Page Size        : " << pageSize << "KB" << endl;
	cout << "Buffer Size      : " << bufferSize << "KB" << endl;
	cout << "Max Page Num     : " << maxNodeNum << endl;
	cout << "Query File       : " << queryfile << endl;

	StrComparator* strcomp;

	int gramSize, gramNum, bucketSize, maxBit;
	int k;
	float threshold;

	if(!strcmp(distPar, "-l"))
	{
		cout << "Linear Scan" << endl;

		strcomp = new StrComparator(MAXLEN);

		LinearScanMethod(strcomp, filename, strNum,
					  queryfile, queryNum);

		delete strcomp;
		return 1;
	}
	// set the order and distance
	if(!strcmp(distPar, "-g"))
	{
		cout << "Gram Order" << endl;
		gramSize = atoi(argv[10]);
		bucketSize = atoi(argv[11]);
		maxBit = atoi(argv[12]);

		cout << "Gram Size        : " << gramSize << endl;
		cout << "Bucket Size      : " << bucketSize << endl;
		cout << "Maxium Bit       : " << maxBit << endl;

		strcomp = new GramDictOrder(gramSize, MAXLEN, bucketSize, maxBit);
	}
	if(!strcmp(distPar, "-ng"))
	{
		cout << "Normalized Gram Order" << endl;
		gramSize = atoi(argv[10]);
		bucketSize = atoi(argv[11]);
		maxBit = atoi(argv[12]);

		cout << "Gram Size        : " << gramSize << endl;
		cout << "Bucket Size      : " << bucketSize << endl;
		cout << "Maxium Bit       : " << maxBit << endl;

		strcomp = new NormGramDictOrder(gramSize, MAXLEN, bucketSize, maxBit);
	}
	if(!strcmp(distPar, "-d"))
	{
		cout << "Dictionary Order" << endl;

		strcomp = new StrDictOrder(MAXLEN);
	}
	if(!strcmp(distPar, "-a"))
	{
		cout << "Gram-Location Join indexing : " << endl;

		gramSize = atoi(argv[10]);
		gramNum = atoi(argv[11]);
		maxBit = atoi(argv[12]);

		cout << "Gram Size        : " << gramSize << endl;
		cout << "Number of Gram   : " << gramNum << endl;
		cout << "maxium bit       : " << maxBit << endl;

		cout << "Gram Location Order" << endl;
		strcomp = new GramLocOrder(gramSize, MAXLEN, gramNum, maxBit, NULL);
	}
	//order and distance done

	if(!strcmp(queryPar, "-rq"))
	{
		cout << "Range Query" << endl;
		BufferManager* bm = new BufferManager(maxNodeNum, bufferSize / pageSize, pageSize);	

		TreeMethod(maxNodeNum, bufferSize, pageSize, strcomp, filename, strNum, bm,
			queryfile, queryNum);

		delete strcomp;
		delete bm;

		return 1;
	}
	if(!strcmp(queryPar, "-kq"))
	{
		cout << "Top-K Query" << endl;

		BufferManager* bm = new BufferManager(maxNodeNum, bufferSize / pageSize, pageSize);	

		TopKTreeMethod(maxNodeNum, bufferSize, pageSize, strcomp, filename, strNum, bm,
			queryfile, queryNum);

		delete strcomp;
		delete bm;

		return 1;
	}
	if(!strcmp(queryPar, "-rj"))
	{
		cout << "Range Join" << endl;
		if(!strcmp(distPar, "-a") || !strcmp(distPar, "-g") || !strcmp(distPar, "-ng"))
		{
			threshold = atof(argv[13]);
		}
		else
		{
			threshold = atof(argv[10]);
		}
		cout << "Threshold : " << threshold << endl;

		BufferManager* bm = new BufferManager(maxNodeNum, bufferSize / pageSize, pageSize);	

		JoinTreeMethod(maxNodeNum, bufferSize, pageSize, strcomp, filename, strNum, bm,
			threshold);

		delete strcomp;
		delete bm;

		return 1;
	}
	if(!strcmp(queryPar, "-kj"))
	{
		cout << "Top-K Join " << gramSize << endl;

		if(!strcmp(distPar, "-a") || !strcmp(distPar, "-g") || !strcmp(distPar, "-ng"))
		{
			k = atoi(argv[13]);
		}
		else
		{
			k = atoi(argv[10]);
		}
		cout << "K          : " << k << endl;

		BufferManager* bm = new BufferManager(maxNodeNum, bufferSize / pageSize, pageSize);	

		TopKJoinTreeMethod(maxNodeNum, bufferSize, pageSize, strcomp, filename, strNum, bm,
			k);

		delete strcomp;
		delete bm;

		return 1;
	}

	return 0;
}
