#ifndef HEAP_H
#define HEAP_H

#include "CoreTypes.h"
#include <vector>
#include <ostream>

namespace Continers
{
	enum HeapType
	{
		MIN_HEAP = 1,
		MAX_HEAP = 2
	};

	/*
	* Binary heap backed by std::vector<T>. Your T better have comparison operators overloaded.
	*/
	template <typename T>
	class Heap
	{
	public:
		Heap(HeapType heapType) : mHeapType(heapType) {}
		std::vector<T>& data() { return mData; }

		// push new elem into heap
		void push(T elem)
		{
			mData.push_back(elem);
			U32 elemIdx = mData.size();
			U32 parentIdx = elemIdx / 2;
			while(parentIdx != 0 && mData[parentIdx-1] > mData[elemIdx-1])
			{
				std::swap(mData[parentIdx-1], mData[elemIdx-1]);
				elemIdx = parentIdx;
				parentIdx = elemIdx/2;
			}
		}

		// pop off top element from heap
		T pop()
		{
			if (mData.empty())
				return 0;

			if (mData.size() == 2)
				if (mData[1] <= mData[0])
					std::swap(mData[0], mData[1]);

			U32 ret = mData[0];
			std::swap(mData[0], mData.back());
			mData.pop_back();

			if (mData.empty())
				return ret;

			U32 elemIdx = 0;
			U32 childIdx = 1;


			while(childIdx < mData.size()-1)
			{
				if (mData[childIdx] <= mData[childIdx+1])
				{
					if (mData[elemIdx] >= mData[childIdx])
						std::swap(mData[elemIdx], mData[childIdx]);
					elemIdx = childIdx;
				}
				else
				{
					if (mData[elemIdx] >= mData[childIdx+1])
						std::swap(mData[elemIdx], mData[childIdx+1]);
					elemIdx = childIdx+1;
				}
				childIdx = elemIdx * 2;
			}

			return ret;
		}

		T top() { return mData.front(); }

		void printHeap()
		{
			for (U32 i = 0; i < mData.size(); ++i)
			{
				cout<<mData[i]<<" ";
			}
			cout<<endl;
		}
	private:
		HeapType mHeapType;
		std::vector<T> mData;
	};
	
}

#endif // HEAP_H