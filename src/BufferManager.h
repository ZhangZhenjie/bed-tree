#ifndef _BUFFER_MANAGER_
#define _BUFFER_MANAGER_


//#include "Page.h"

#include <ctime>
#include <fstream>


using namespace std;

struct BufEntry
{
	void*   pointer;
	int     nodeID;
	long  last_time;
	bool	dirtyBit;
	bool	memBit;
}
typedef BufTabEntry;

class BufferManager
{
public:
	BufferManager(int size, int constraint, int pSize);
	~BufferManager();

	bool AddEntry(void* pointer, int id);
	void LoadUpdate(int id);
	void ReadUpdate(int id);
	void WriteUpdate(int id);
	void SaveUpdate(int id);

	int RequestID();

	int GetIOTime(){return ioTime;};

	bool isDirty(int id);
	bool CopyFromDisk(int id, char* buffer);
	bool SaveToDisk(int id, char* buffer);

	int GetCurSize(){return curSize;};

	//void CheckConsistency();
private:

	fstream diskfile;
	BufTabEntry* entryArray;
	int maxSize;
	int curSize;
	int openPageNum;

	int pageConstraint;

	long ioTime;

	int PickupSaveEntry(int curID);

	long updateTime;

	int pageSize;
};

#endif

