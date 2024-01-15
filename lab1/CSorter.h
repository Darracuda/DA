#pragma once
#include "TPair.h"
#include "CDate.h"

using pair_t = TPair<CDate, string>;
typedef uint64_t (*selector_t)(pair_t& pair);

class CSorter
{
private:
	static uint64_t GetMax(pair_t array[], const size_t size, selector_t selector);

public:
	static void RadixSort(pair_t array[], const size_t size, selector_t selector);
	static void CountSort(pair_t array[], const size_t size, selector_t selector, uint64_t exp);
};


//https://www.geeksforgeeks.org/radix-sort/

