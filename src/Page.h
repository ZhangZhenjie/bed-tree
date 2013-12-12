#ifndef _PAGE_H_
#define _PAGE_H_

#include "BufferManager.h"

class Page
{
public:
	Page(int size, BufferManager* manager);
	virtual ~Page();


	int GetID(){return nodeID;};

	bool LoadFromFile();
	bool SaveToFile();
private:
	bool isInMem;

protected:
	int pageSize;
	int nodeID;

	char* buffer;
	BufferManager* bm;

};

#endif

