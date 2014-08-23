#ifndef MATRIX2D_H
#define MATRIX2D_H

#include "CoreTypes.h"
#include <string>

namespace core {

	template <typename T>
	class Matrix
	{
	public:
		Matrix() : mRows(), mColumns(), mElems(NULL) {};

		Matrix(size_t rows, size_t columns) : mRows(rows), mColumns(columns), mElems(new T[rows * columns]()) {}

		void allocate(size_t rows, size_t columns)
		{
			mRows = rows;
			mColumns = columns;
			mElems = new T[rows * columns];
		}

		~Matrix() {delete[] mElems;}

		inline T* operator[](const size_t row) // Setter
		{
			return mElems + (row*mColumns);
		}

		inline const T* operator[](const size_t row) const // Getter
		{
			return mElems + (row*mColumns);
		}

		inline int getRows() { return mRows; }

		inline int getColumns() { return mColumns; }

		inline T* data() { return mElems; }

		void print();

		void InitWithRandom();
	private:
		size_t mRows, mColumns;
		T* mElems;
	};

	typedef core::Matrix<int> Grid;
}

#endif // MATRIX2D_H