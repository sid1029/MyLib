#ifndef HEAP_H
#define HEAP_H

#include "CoreTypes.h"
#include <vector>
#include <ostream>
#include <functional>

namespace Continers
{
	
	enum HeapType
	{
		MIN_HEAP = 1, // Operator less_equal
		MAX_HEAP = 2 // Operator greater_equal
	};

	/*
	* Binary heap backed by std::vector<T>. Your T better have comparison operators overloaded.
	* O(log(n)) insertion and deletion times.
	*/
	template <typename T>
	class Heap
	{
	public:
		Heap(HeapType heapType) : mHeapType(heapType)
		{
			mData.resize(1);
			if (heapType == MIN_HEAP)
				mComparator = std::less_equal<T>();
			else
				mComparator = std::greater<T>();
		}

		std::vector<T>& data() { return mData; }

		U32 size() { return mData.size() - 1; }

		// push new elem into heap
		void push(T elem)
		{
			if (mData.size() <= 1)
			{
				mData.insert(mData.begin() + 1, elem);
			}
			else
			{
				mData.push_back(elem);
				bubbleUp(mData.size() - 1);
			}
		}

		// pop off top element from heap
		T pop()
		{
			if (size() < 1)
				return T();
			size_t heapSize = mData.size();
			T ret = *(mData.begin() + 1);
			std::swap(mData.back(), mData[1]);
			mData.erase(mData.end() - 1);
			percolateDown(1);
			return ret;
		}

		T& top() { return mData.front() + 1; }

		void printHeap()
		{
			for (U32 i = 1; i < mData.size(); ++i)
			{
				std::cout<<mData[i]<<" ";
			}
			std::cout << std::endl;
		}
	private:
		HeapType mHeapType;
		std::vector<T> mData;
		std::function<bool (T, T)> mComparator;

		void bubbleUp(size_t idx)
		{
			size_t parent = (size_t)std::floor(idx / 2);
			while (parent >= 1 && mComparator(mData[idx], mData[parent]))
			{
				std::swap(mData[idx], mData[parent]);
				idx = parent;
				parent = (size_t)std::floor(parent / 2);
			}
		}

		void percolateDown(size_t idx)
		{
			size_t leftChild = idx * 2;
			size_t rightChild = idx * 2 + 1;
			size_t heapSize = mData.size();

			if ( heapSize == 3 && !mComparator(mData[idx], mData[leftChild]) ) // Theres only 2 elements in the heap. Directly order them.
			{
				std::swap(mData[idx], mData[leftChild]);
				return;
			}

			while ( idx < heapSize && leftChild < heapSize && rightChild < heapSize && 
				( !mComparator(mData[idx], mData[leftChild]) || !mComparator(mData[idx], mData[rightChild]) ) )
			{
				if ( mComparator(mData[leftChild], mData[rightChild]) )
				{
					std::swap(mData[idx], mData[leftChild]);
					idx = leftChild;
				}
				else
				{
					std::swap(mData[idx], mData[rightChild]);
					idx = rightChild;
				}
				leftChild = idx * 2;
				rightChild = idx * 2 + 1;
			}
		}
	};
	
}

#endif // HEAP_H