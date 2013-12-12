#include "BufferManager.h"

#include <ctime>
#include <iostream>

#include "Page.h"

using namespace std;

BufferManager::BufferManager(int size, int constraint, int pSize)
{
	maxSize = size;
	
	entryArray = new BufTabEntry[maxSize];

	int i;

	for(i = 0; i < maxSize; i++)
	{
		entryArray[i].nodeID = -1;
	}

	curSize = 0;
	openPageNum = 0;

	pageConstraint = constraint;

	ioTime = 0;

	cout << "Buffer manager created : " << maxSize << " " << pageConstraint << endl;

	updateTime = 0;

	pageSize = pSize;

	diskfile.open("disk.dat", ios::out);

	char* buffer = new char[pageSize * 1024];

	for(i = 0; i < maxSize; i++)
	{
		diskfile.write(buffer, pageSize);
	}

	diskfile.close();

	diskfile.open("disk.dat", ios::in | ios::out | ios::binary);

	pageSize *= 1024;

	delete [] buffer;
}

BufferManager::~BufferManager()
{

	int i;

	for(i = 0; i < maxSize; i++)
	{
		if(entryArray[i].nodeID != -1)
		{
			delete (Page*)(entryArray[i].pointer);
		}
	}

	delete [] entryArray;

	diskfile.close();
}

bool BufferManager::CopyFromDisk(int id, char* buffer)
{
	if(entryArray[id].nodeID == -1)
		return false;

	int pos = id * pageSize;

	diskfile.seekg(pos, ios::beg);

	diskfile.read(buffer, pageSize);

	return true;
}

bool BufferManager::SaveToDisk(int id, char* buffer)
{
	if(entryArray[id].nodeID == -1)
		return false;

	int pos = id * pageSize;

	diskfile.seekp(pos, ios::beg);

	diskfile.write(buffer, pageSize);

	return true;
}

bool BufferManager::AddEntry(void* pointer, int id)
{
	if(id < 0 || id >= maxSize){
		cout << "Add Entry : Incorrect ID " <<  id << endl;
		return false;
	}

	entryArray[id].pointer = pointer;
	entryArray[id].nodeID = id;
	entryArray[id].memBit = true;
	entryArray[id].dirtyBit = true;
	entryArray[id].last_time = updateTime++;

	openPageNum++;

	if(openPageNum > pageConstraint)
	{
		int saveEntry = PickupSaveEntry(id);
		
		if(!((Page*)(entryArray[saveEntry].pointer))->SaveToFile())
		{
			cout << "save error" << endl;
		}
	}

	return true;
}

void BufferManager::LoadUpdate(int id)
{
	if(id >= maxSize)
	{
		cout << "Load : Incorrect ID " << id << endl;
		return;
	}

	entryArray[id].last_time = updateTime++;
	entryArray[id].memBit = true;
	entryArray[id].dirtyBit = false;

	//if(ioTime == 4577)
	//	cout << "let's do it" << endl;
	//cout << ioTime << " Page " << id << " loads" << endl;
	ioTime++;

	openPageNum++;

	if(openPageNum > pageConstraint)
	{
		int saveEntry = PickupSaveEntry(id);
		
		if(!((Page*)(entryArray[saveEntry].pointer))->SaveToFile())
		{

			cout << "file saving error" << endl;
		}

	}
}

void BufferManager::SaveUpdate(int id)
{
	if(id < 0 || id >= maxSize)
	{
		cout << "Save : Incorrect ID " << id << endl;
		return;
	}

	entryArray[id].memBit = false;

	openPageNum--;

	//if(id == 1347)
	//	id = id;

	//cout << ioTime << " Page " << id << " saves" << endl;
	ioTime++;

}

void BufferManager::WriteUpdate(int id)
{
	if(id < 0 || id >= maxSize)
	{
		cout << "Write : Incorrect ID " << id << endl;
		return;
	}

	entryArray[id].last_time = updateTime++;
	//entryArray[id].memBit = true;
	entryArray[id].dirtyBit = true;
}

void BufferManager::ReadUpdate(int id)
{
	if(id >= maxSize)
	{
		cout << "Read : Incorrect ID " << id << endl;
		return;
	}

	entryArray[id].last_time = updateTime++;
	//entryArray[id].memBit = true;
}

int BufferManager::PickupSaveEntry(int curID)
{
	//int minCleanEntry = -1;
	int minEntry = -1;
	//time_t earlyCleanTime;
	time_t earlyTime;

	int i;

	for(i = 0; i < maxSize; i++)
	{
		if(i == curID)
			continue;

		if(entryArray[i].nodeID == -1 || !entryArray[i].memBit)
			continue;

		if(minEntry == -1)
		{
			minEntry = i;
			earlyTime = entryArray[i].last_time;
		}
		else if(entryArray[i].last_time < earlyTime)
		{
			minEntry = i;
			earlyTime = entryArray[i].last_time;
		}

		/*
		if(!entryArray[i].dirtyBit)
		{
			if(minCleanEntry == -1)
			{
				minCleanEntry = i;
				earlyCleanTime = entryArray[i].last_time;
			}
			else if(entryArray[i].last_time < earlyCleanTime)
			{
				earlyCleanTime = entryArray[i].last_time;
			}
		}
		*/
	}

	//if(minCleanEntry != -1)
	//	return minCleanEntry;

	return minEntry;
}


int BufferManager::RequestID()
{
	if(curSize == maxSize)
	{
		cout << "new page number : " << maxSize * 2 << endl;

		int newMaxSize = maxSize * 2;

		BufTabEntry* newEntryArray = new BufTabEntry[newMaxSize];

		int i;

		for(i = 0; i < maxSize; i++)
		{
			newEntryArray[i].pointer = entryArray[i].pointer;
			newEntryArray[i].last_time = entryArray[i].last_time;
			newEntryArray[i].memBit = entryArray[i].memBit;
			newEntryArray[i].nodeID = entryArray[i].nodeID;
			newEntryArray[i].dirtyBit = entryArray[i].dirtyBit;
		}

		for(i = maxSize; i < newMaxSize; i++)
			newEntryArray[i].nodeID = -1;

		delete [] entryArray;
		entryArray = newEntryArray;
		maxSize = newMaxSize;
		
		//cout << "new maximal page size : " << maxSize << endl;

		//return -1;
	}

	curSize++;

	return curSize - 1;
}

bool BufferManager::isDirty(int id)
{
	if(id < 0 || id >= maxSize)
	{
		cout << "wrong id " << endl;
		return false;
	}

	return entryArray[id].dirtyBit;
}

/*
void BufferManager::CheckConsistency()
{
	int i;

	for(i = 0; i < maxSize; i++)
	{
		if(entryArray[i].nodeID != -1)
		{
			if(((Page*)(entryArray[i].pointer))->isInMem && (!entryArray[i].memBit))
			{
				cout << "error" << endl;
			}
			if(! ((Page*)(entryArray[i].pointer))->isInMem && (entryArray[i].memBit))
			{
				cout << "error" << endl;
			}
		}
	}
}
*/


