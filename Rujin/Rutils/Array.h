#ifdef RUTILS_ARRAY
namespace rutils
{
	void PrintElementsAsGrid(int* numbers, int size, int cols);
	int GetGridIndex(int rowIdx, int colIdx, int nrCols);
	void InitializeGridArray(int  grid[], int size, int rows, int columns);
	void BubbleSort(int array[], int elementCount);
	int MinElement(int array[], unsigned int size);
	int MaxElement(int array[], unsigned int size);


	//applicable on any array
	template <typename T>
	void Shuffle(T array[], unsigned int size, int swapIterations)
	{
		for (int i = 0; i < swapIterations; i++)
		{
			int idx1{};
			int idx2{};
			do {
				idx1 = RandomIntFromInterval(0, size / sizeof(array[0]) - 1);
				idx2 = RandomIntFromInterval(0, size / sizeof(array[0]) - 1);
			} while (idx1 == idx2);

			Swap(array, idx1, idx2);
		}
	}

	template <typename T>
	void ShiftArrayRight(T array[], int size, int amount)
	{
		for (int iteration = 0; iteration < amount; iteration++)
		{
			//to the right
			for (int i = size / sizeof(array[0]) - 2; i >= 0; i--)
			{
				array[i + 1] = array[i]; //move all element to the right except last one
			}
			array[0] = T{};
		}
	}

	template <typename T>
	void Swap(T array[], int idx1, int idx2)
	{
		T temp = array[idx1];
		array[idx1] = array[idx2];
		array[idx2] = temp;
	}


	template <typename T>
	void PrintElements(T array[], int size)
	{
		for (unsigned int i = 0; i < size / sizeof(array[0]); i++)
		{
			std::cout << array[i] << " ";
		}
		std::cout << std::endl;
	}

	template <typename T>
	void PrintElements(T array[], int startIdx, int endIdx)
	{
		for (int i = startIdx; i <= endIdx; i++)
		{
			std::cout << array[i] << " ";
		}
		std::cout << std::endl;
	}

	template <typename T>
	int Count(T array[], unsigned int size, T reference)
	{
		int count = 0;
		for (unsigned int i = 0; i < size / sizeof(array[0]); i++)
		{
			if (array[i] == reference) count++;
		}

		return count;
	}
}
#endif // RUTILS_ARRAY
