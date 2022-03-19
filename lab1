#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

using namespace std;

// -- TPair
template <typename TKey, typename TValue>
class TPair
{
private:
	TKey _key;
	TValue _value;

public:
	TPair() = default;
	TPair(const TPair& other);
	TPair(const TKey& key, const TValue& value);
	virtual ~TPair();

	TKey GetKey();
	TValue GetValue();
};

template<typename TKey, typename TValue>
TPair<TKey, TValue>::TPair(const TPair& other)
{
	_key = other._key;
	_value = other._value;
}

template<typename TKey, typename TValue>
TPair<TKey, TValue>::TPair(const TKey& key, const TValue& value)
{
	_key = key;
	_value = value;
}

template<typename TKey, typename TValue>
TPair<TKey, TValue>::~TPair()
{
}

template<typename TKey, typename TValue>
TKey TPair<TKey, TValue>::GetKey()
{
	return _key;
}

template<typename TKey, typename TValue>
TValue TPair<TKey, TValue>::GetValue()
{
	return _value;
}

// -- CSorter
using pair_t = TPair<uint32_t, const char *>;

class CSorter
{
private:
	static uint32_t GetMax(pair_t array[], const size_t size);

public:
	static void RadixSort(pair_t array[], const size_t size);
	static void CountSort(pair_t array[], const size_t size, uint32_t exp);
};

uint32_t CSorter::GetMax(pair_t array[], const size_t size)
{
	uint32_t max = array[0].GetKey();
	for (size_t i = 1; i < size; i++)
	{
		uint32_t item = array[i].GetKey();
		if (item > max)
			max = item;
	}
	return max;
}

void CSorter::RadixSort(pair_t array[], const size_t size)
{
	uint32_t maximum = GetMax(array, size);

	for (uint32_t divisor = 1; maximum / divisor > 0; divisor *= 10)
		CountSort(array, size, divisor);
}

void CSorter::CountSort(pair_t array[], const size_t size, uint32_t divisor)
{
	int count[10] = { 0 };  // initialize 10 elements

	// calculate occurrence of digits
	for (int i = 0; i < size; i++)
	{
		uint32_t digit = (array[i].GetKey() / divisor) % 10;
		count[digit]++;
	}

	// consider occurrence of previous digits 
	for (int i = 1; i < 10; i++)
		count[i] += count[i - 1];

	pair_t* output = new pair_t[size];  // output array

	// arrange the output array and change counts
	for (int i = (int)size - 1; i >= 0; i--)
	{
		uint32_t digit = (array[i].GetKey() / divisor) % 10;
		output[count[digit] - 1] = array[i];
		count[digit]--;
	}

	// prepare results
	for (int i = 0; i < size; i++)
		array[i] = output[i];

    delete [] output;
}

// -- main
pair_t ParseLine(const string& line)
{
//	size_t pos = line.find('\t');
//	if (pos == string::npos)
//		throw runtime_error("main, exception: not tab delimiter in input line");

    uint32_t year;
    uint32_t month;
    uint32_t day;

    // parse input string
    ::sscanf(line.c_str(), "%d.%d.%d", &day, &month, &year);

    // check values
//    if ((1 > day) || (day > 31) || (1 > month) || (month > 12) || (1 > year))
//        throw runtime_error("exception: invalid date format");

    // example 20220224 for 24.02.2022
//    uint32_t key = day + 100 * (month + 100 * year);
    uint32_t key = day | (month << 5) | (year << 9);

	pair_t pair(key, line.c_str());
	return pair;
}

int main()
{
	// read lines from console
	vector<string> lines;
	string line;
	while (getline(cin, line))
	{
        line.shrink_to_fit();
		if (!line.empty())
			lines.push_back(line);
	}

	// parse lines to pairs
	size_t size = lines.size();
	pair_t* array = new pair_t[size];
	for (size_t i = 0; i < size; i++)
	{
		array[i] = ParseLine(lines[i]);
	}
    
	// sorting
	CSorter::RadixSort(array, size);

	// output results
	for (size_t i = 0; i < size; i++)
	{
        puts(array[i].GetValue());
	}
}

