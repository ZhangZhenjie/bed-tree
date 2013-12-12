#include "Page.h"

#include <iostream>

using namespace std;

Page::Page(int size, BufferManager* manager)
{
	bm = manager;
	pageSize = size;

	buffer = new char[pageSize];

	isInMem = true;
}

Page::~Page()
{
	if(isInMem)
	{
		delete [] buffer;
	}
}

bool Page::LoadFromFile()
{
	if(isInMem)
		return true;
	/*
	char filename[100];

	sprintf(filename, "temp\\%d.dat", nodeID);

	ifstream input(filename, ios::in | ios::binary);
	*/

	if(!buffer)
		buffer = new char[pageSize];
	else
	{
		cout << "loading error" << endl;
		//input.close();
		return false;
	}

	//input.read(buffer, pageSize);

	//input.close();

	if(!bm->CopyFromDisk(nodeID, buffer))
		cout << "reading error" << endl;

	bm->LoadUpdate(nodeID);

	isInMem = true;

	return true;
}

bool Page::SaveToFile()
{
	if(!isInMem)
		return false;

	/*
	char filename[100];

	sprintf(filename, "temp\\%d.dat", nodeID);
	*/

	if(bm->isDirty(nodeID))
	{
		//ofstream output(filename, ios::out | ios::binary);

		//output.write(buffer, pageSize);

		//output.close();

		bm->SaveToDisk(nodeID, buffer);
	}

	isInMem = false;
	bm->SaveUpdate(nodeID);


	delete [] buffer;

	buffer = NULL;

	return true;
}
