#include "CSorter.h"

uint64_t CSorter::GetMax(pair_t array[], const size_t size, selector_t selector)
{
	uint64_t max = selector(array[0]);
	for (size_t i = 0; i < size; i++)
	{
		uint64_t item = selector(array[i]);
		if (item > max)
			max = item;
	}
	return max;
}

void CSorter::RadixSort(pair_t array[], const size_t size, selector_t selector)
{
	uint64_t maximum = GetMax(array, size, selector);
	
	for (uint64_t divisor = 1; maximum / divisor > 0; divisor *= 10)
		CountSort(array, size, selector, divisor);
}

void CSorter::CountSort(pair_t array[], const size_t size, selector_t selector, uint64_t divisor)
{
    int count[10] = { 0 };  // initialize 10 elements

    // calculate ocurrence of digits
    for (size_t i = 0; i < size; i++)
        count[(selector(array[i]) / divisor) % 10]++;

    // consider occurence of previous digits 
    for (size_t i = 1; i < 10; i++)
        count[i] += count[i - 1];

    pair_t* output = new pair_t[size];  // output array

    // arrange the output array and change counts
    for (size_t i = 0; i < size; i++)
    {
        output[count[(selector(array[i]) / divisor) % 10] - 1] = array[i];
        count[(selector(array[i]) / divisor) % 10]--;
    }

    // prepare results
    for (size_t i = 0; i < size; i++)
        array[i] = output[i];
}
