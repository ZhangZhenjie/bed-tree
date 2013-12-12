#ifndef _QUEUE_H_
#define _QUEUE_H_

class Queue
{
public:
	Queue(int size);
	~Queue();

	bool Enqueue(void* pointer, double value);
	bool Dequeue(void** pp, double* value);
	bool Dequeue();

	void Print();

	int GetCurSize(){return curSize;};

	void** GetCurArray(){return array;};

	double GetTopValue();
private:
	int maxSize;
	void** array;
	double* values;

	int curSize;

	int Parent(int pos);
	int LChild(int pos);
	int RChild(int pos);

	int Adjust(int pos);
};

#endif

