#include "RingBuffer.h"
#include <iostream>
#include <assert.h>

namespace rb {

using namespace std;

RingBuffer::RingBuffer() : mBuffSize(0), mFillCount(0)
{
	this->start = NULL;
	this->end = NULL;
	this->buffer = NULL;
}

RingBuffer::RingBuffer(const int buffSize)
{
	mBuffSize = buffSize;
	buffer = new int[mBuffSize];
	assert(buffer);
	std::memset(buffer, 0, mBuffSize * sizeof(int));
	start = end = buffer;
	mFillCount = 0;
	cout<<"Buffer of fixed size "<<mBuffSize<<" created.\n";
}

void RingBuffer::push(const int elem)
{
	if ((end == start) && (mFillCount != 0))
		return;
	else
	{
		if (end == (buffer + mBuffSize))
			end = buffer;
		*end = elem;
		//cout<<"Element added : "<<*end<<"\n";
		++mFillCount;
		if (end == (buffer + mBuffSize))
		{
			end = buffer;
		}
		else
			end++;
	}
}

int RingBuffer::pop()
{
	if ((start == end) && mFillCount == 0)
	{
		cout<<"Reached end of buffer. Nothing to delete\n";
		return 0;
	}
	if (start == (buffer + mBuffSize))
		start = buffer;

	int val = *start;
	*start = 0;
	start++;
	
	cout<<"Element deleted : "<<val<<"\n";
	--mFillCount;
	return val;
}

void RingBuffer::printbuffer()
{
	int* iter = buffer;
	//cout<<"Buffer has "<<mFillCount<<" elements :\n";
	
	while (iter != (buffer + mBuffSize))
	{
		cout<<*iter<<" ";
		iter++;
	}
	
	cout<<endl;
}

} // namespace rb