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

	typedef std::pair<std::pair<U64, U64>, U64> AppPair;
	static std::map<std::pair<U64, U64>, U64> appMap;
	static U32 readCount = 0, writeCount = 0;
	U64 MaxApplesExhaustive(Matrix& m, U64 i, U64 j)
	{
		U64 rows = m.getRows(), cols = m.getColumns();
		if (i >= rows || j >= cols)
			return 0;
		
		// Cache in map
		if (appMap.count(std::pair<U64, U64>(i+1, j)) == 0)
		{
			++writeCount;
			appMap.insert(AppPair(std::pair<U64, U64>(i+1, j), MaxApplesExhaustive(m, i+1, j)));
		}
		if (appMap.count(std::pair<U64, U64>(i, j + 1)) == 0)
		{
			++writeCount;
			appMap.insert(AppPair(std::pair<U64, U64>(i, j + 1), MaxApplesExhaustive(m, i, j+1)));
		}

		// Return values from map
		if (i == rows - 1)
		{
			++readCount;
			return m[i][j] + appMap[std::pair<U64, U64>(i, j+1)];
		}
		if (j == cols - 1)
		{
			++readCount;
			return m[i][j] + appMap[std::pair<U64, U64>(i+1, j)];
		}
		
		readCount += 2;
		return std::max(m[i][j] + appMap[std::pair<U64, U64>(i+1, j)], m[i][j] + appMap[std::pair<U64, U64>(i, j+1)]);
	}

	void PrintRW()
	{
		std::cout<<"\n\nreadCount = "<<readCount<<" writeCount = "<<writeCount;
	}

}