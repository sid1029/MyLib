#ifndef RINGBUFFER_H
#define RINGBUFFER_H

namespace rb {

class RingBuffer
{
public:
	RingBuffer();
	RingBuffer(const int buffSize);
	void push(const int elem);
	int pop();
	void printbuffer();
private:
	int* start;
	int* end;
	int* buffer;
	int mBuffSize;
	int mFillCount;
};

}
#endif // RINGBUFFER_H