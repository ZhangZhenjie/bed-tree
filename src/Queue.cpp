#include "Queue.h"
#include <iostream>

using namespace std;

Queue::Queue(int size)
{
	maxSize = size;

	array = new void*[maxSize];
	values = new double[maxSize];

	curSize = 0;
}

Queue::~Queue()
{
	delete [] array;
	delete [] values;
}

bool Queue::Enqueue(void* pointer, double value)
{
	if(curSize == maxSize){
		cout << "full queue!" << endl;
		return false;
	}

	array[curSize] = pointer;
	values[curSize] = value;

	curSize++;

	int curpar = Parent(curSize - 1);

	while(curpar >= 0){
		Adjust(curpar);

		curpar = Parent(curpar);
	}
	return true;
}

int Queue::Parent(int pos)
{
	if(pos == 0)
		return -1;

	return (pos - 1) / 2;
}

int Queue::LChild(int pos)
{
	int newpos = pos * 2 + 1;

	if(newpos < curSize)
		return newpos;
	else
		return -1;
}

int Queue::RChild(int pos)
{
	int newpos = pos * 2 + 2;

	if(newpos < curSize)
		return newpos;
	return -1;
}

int Queue::Adjust(int pos)
{
	if(pos < 0 || pos >= curSize)
		return -1;

	int lpos = LChild(pos);
	int rpos = RChild(pos);

	void* temp;
	double tempv;

	int tpos;
	double tvalue;

	if(rpos == -1){
		if(lpos == -1){
			return -1;
		}
		else{
			tpos = lpos;
			tvalue = values[lpos];
		}
	}
	else{
		tpos = values[lpos]>values[rpos]?rpos:lpos;
		tvalue = values[tpos];
	}

	if(tvalue < values[pos]){
		temp = array[pos];
		array[pos] = array[tpos];
		array[tpos] = temp;

		tempv = values[pos];
		values[pos] = values[tpos];
		values[tpos] = tempv;
	}

	return tpos;
}

bool Queue::Dequeue(void** pp, double* value)
{
	if(curSize == 0)
		return false;

	*pp = array[0];
	*value = values[0];

	array[0] = array[curSize - 1];
	values[0] = values[curSize - 1];

	curSize--;

	int curPos = 0;

	while(curPos >= 0)
	{
		curPos = Adjust(curPos);
	}

	return true;
}

bool Queue::Dequeue()
{
	if(curSize == 0)
		return false;

	//*pp = array[0];
	//*value = values[0];

	array[0] = array[curSize - 1];
	values[0] = values[curSize - 1];

	curSize--;

	int curPos = 0;

	while(curPos >= 0)
	{
		curPos = Adjust(curPos);
	}

	return true;
}

void Queue::Print()
{
	int i;

	for(i = 0; i < curSize; i++){
		cout << values[i] << " ";
	}
	cout << endl;
}

double Queue::GetTopValue()
{
	if(curSize == 0)
		return 100000;

	else
		return values[0];
}
