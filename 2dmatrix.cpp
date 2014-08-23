#include "2dmatrix.h"
#include <iostream>
#include <map>
#include <iomanip>

namespace core {
	void Matrix::InitWithRandom()
	{
		time_t t;
		time(&t);
		std::srand((U32)t);
		for(U64 i = 0; i < mRows; ++i)
			for (U64 j = 0; j < mColumns; ++j)
				mElems[i * mColumns + j] = std::rand() % (mRows + mColumns);
	}

	void Matrix::print()
	{
		for(U64 i = 0; i < mRows; ++i)
		{
			for (U64 j = 0; j < mColumns; ++j)
			{
				std::cout<<std::setw(4)<<mElems[i * mColumns + j]<<" ";
			}
			std::cout<<"\n";
		}
	}
}