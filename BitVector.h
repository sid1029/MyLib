#ifndef BIT_VECTOR_H
#define BIT_VECTOR_H

#include "CoreTypes.h"
#include <limits>
#include <memory>

using namespace std;

namespace Containers
{
	// A growable array of booleans that takes 1 bit per boolean.
	// The array grows only, does not shrink. Clear() will empty the array releasing all space back to the heap.
	class BitVector
	{
		// Allocate 4 bytes by default. Init all to false.
		BitVector() : mNumAllocBytes(4), mBytes(new U08[4]), mLength(32)
		{
			memset(mBytes, 0, mNumAllocBytes * CHAR_BIT);
		}

		BitVector(U32 size, bool initAll = false)
		{
			mLength = size;
			// For a requested size of 16 allocate only 2 bytes, but for a requested size of 19 allocate 2 + 1 bytes.
			mNumAllocBytes = (size % CHAR_BIT == 0) ? (size / CHAR_BIT) : ((size / CHAR_BIT) + 1);

			mBytes = new U08[mNumAllocBytes];
			if (initAll)
				memset(mBytes, 1, mLength);
			else
				memset(mBytes, 0, mLength);
		}

		bool operator[] (const U64 index) const
		{
			if (index > mLength) return false;

			U08 shift = (1 << (index % CHAR_BIT));

			return (bool)(mBytes[index / CHAR_BIT] & shift);
		}

	private:
		U64		mNumAllocBytes;	// Number of bytes allocated currently.
		U64		mLength;		// Number of booleans stored. (exposed to user)
		U08*	mBytes;			// Actual storage space thats allocated.
	};
}
#endif // BIT_VECTOR_H