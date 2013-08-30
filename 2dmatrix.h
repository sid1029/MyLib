#ifndef MATRIX2D_H
#define MATRIX2D_H

#include "CoreTypes.h"

namespace core {

	class Matrix {
	public:
		Matrix();

		Matrix(U64 rows, U64 columns) : mRows(rows), mColumns(columns), mElems(new U64[rows * columns]) {}

		~Matrix() {delete[] mElems;}

		void InitWithRandom();
		U64* operator[](const U64 row) // Setter
		{
			return mElems + (row*mColumns);
		};

		const U64* operator[](const U64 row) const // Getter
		{
			return mElems + (row*mColumns);
		};

		U64 getRows() { return mRows; }

		U64 getColumns() { return mColumns; }

		void print();
	private:
		U64 mRows, mColumns;
		U64* mElems;
	};

	U64 MaxApplesExhaustive(Matrix& m, U64 i, U64 j);
	void PrintRW();
}

#endif // MATRIX2D_H