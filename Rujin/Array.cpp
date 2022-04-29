#include "stdafx.h"

#include "Array.h"
#ifdef RUTILS_ARRAY
void rutils::PrintElementsAsGrid(int* numbers, int size, int cols)
{
	for (unsigned int i = 0; i < size / sizeof(numbers[0]); i++)
	{
		std::cout << numbers[i] << " ";
		if (i != 0 && (i + 1) % cols == 0) std::cout << std::endl;
	}
	std::cout << std::endl;
}

int rutils::GetGridIndex(int rowIdx, int colIdx, int nrCols)
{
	return rowIdx * nrCols + colIdx;
}

void rutils::InitializeGridArray(int  grid[], int size, int rows, int columns)
{
	//imo, my preffered method... (but i will make the excercise your way nontheless)
	//for (int i = 0; i < size / sizeof(grid[0]); i++)
	//{
	//   
	//    grid[i] = (i / columns + 1) * 10;  //CalculateRowNr (in tientallen)
	//    grid[i] += i % columns + 1; // calculate colNr (eenheden)
	//}

	//some future reference code:
	/*int cols = 2;
	int rows = 2;
	for (size_t i = 0; i < m_TileCount; i++)
	{
		int col = i % rows + 1;
		int row = i / cols + 1;
	}*/


	for (int row = 1; row <= rows; row++)
	{
		for (int column = 1; column <= columns; column++)
		{
			grid[GetGridIndex(row - 1, column - 1, columns)] = row * 10 + column;
		}
	}
}
int rutils::MinElement(int array[], unsigned int size)
{
	int smallestValue = array[0];

	for (unsigned int i = 0; i < size / sizeof(array[0]); i++)
	{
		if (array[i] < smallestValue) smallestValue = array[i];
	}

	return smallestValue;
}

int rutils::MaxElement(int array[], unsigned int size)
{
	int biggestValue = array[0];

	for (unsigned int i = 0; i < size / sizeof(array[0]); i++)
	{
		if (array[i] > biggestValue) biggestValue = array[i];
	}

	return biggestValue;
}


void rutils::BubbleSort(int array[], int elementCount)
{
	//iterate and swap if needed
	for (int i = 1; i < elementCount; i++)
	{
		if (array[i - 1] > array[i]) Swap(array, i - 1, i);
	}

	//perform recursion
	if (elementCount - 1 <= 0) return; // recursion end check
	BubbleSort(array, elementCount - 1);
}

#endif // RUTILS_ARRAY


