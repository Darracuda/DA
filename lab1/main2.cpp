#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

using namespace std;

// -- CDate
class CDate
{
private:
	uint64_t _internalValue = 0;
	uint16_t _year = 0;
	uint16_t _month = 0;
	uint16_t _day = 0;
	string _inputString;

public:
	CDate() = default;
	CDate(const CDate& other);
	CDate(const uint16_t& year, const uint16_t& month, const uint16_t& day, const string& inputString);

	virtual ~CDate();

	static CDate FromString(const string& input);

	uint16_t GetYear();
	uint16_t GetMonth();
	uint16_t GetDay();

	string GetInputString();
	string GetFormattedString();

	uint64_t GetInternalValue();
};

CDate::CDate(const CDate& other)
{
	_year = other._year;
	_month = other._month;
	_day = other._day;
	_inputString = other._inputString;
	_internalValue = other._internalValue;
}

CDate::CDate(const uint16_t& year, const uint16_t& month, const uint16_t& day, const string& inputString)
{
	// example 20220224 for 24.02.2022
	_internalValue = (uint64_t)day + 100 * ((uint64_t)month + 100 * (uint64_t)year);

	// date components
	_year = year;
	_month = month;
	_day = day;

	// input string
	_inputString = inputString;
}

CDate::~CDate()
{
}

CDate CDate::FromString(const string& input)
{
	int year;
	int month;
	int day;

	// parse input string
	::sscanf(input.c_str(), "%d.%d.%d", &day, &month, &year);

	// check values
	if ((1 > day) || (day > 31) || (1 > month) || (month > 12) || (1 > year))
		throw runtime_error("CDate, exception: invalid date format");

	// create date object
	CDate date(year, month, day, input);
	return date;
}

uint16_t CDate::GetYear()
{
	return _year;
}

uint16_t CDate::GetMonth()
{
	return _month;
}

uint16_t CDate::GetDay()
{
	return _day;
}

string CDate::GetInputString()
{
	return _inputString;
}

uint64_t CDate::GetInternalValue()
{
	return _internalValue;
}

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

template class TPair<int, string>;
template class TPair<CDate, string>;

// -- CSorter
using pair_t = TPair<CDate, const char *>;
typedef uint64_t(*selector_t)(pair_t& pair);

class CSorter
{
private:
	static uint64_t GetMax(pair_t array[], const size_t size, selector_t selector);

public:
	static void RadixSort(pair_t array[], const size_t size, selector_t selector);
	static void CountSort(pair_t array[], const size_t size, selector_t selector, uint64_t exp);
};

uint64_t CSorter::GetMax(pair_t array[], const size_t size, selector_t selector)
{
	uint64_t max = selector(array[0]);
	for (size_t i = 1; i < size; i++)
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
	for (int i = 0; i < size; i++)
	{
		uint64_t digit = (selector(array[i]) / divisor) % 10;
		count[digit]++;
	}

	// consider occurence of previous digits 
	for (int i = 1; i < 10; i++)
		count[i] += count[i - 1];

	pair_t* output = new pair_t[size];  // output array

	// arrange the output array and change counts
	for (int i = (int)size - 1; i >= 0; i--)
	{
		uint64_t digit = (selector(array[i]) / divisor) % 10;
		output[count[digit] - 1] = array[i];
		count[digit]--;
	}

	// prepare results
	for (int i = 0; i < size; i++)
		array[i] = output[i];
}

// -- main
pair_t ParseLine(const string& line)
{
	size_t pos = line.find('\t');
	if (pos == string::npos)
		throw runtime_error("main, exception: not tab delimiter in input line");
	string part1 = line.substr(0, pos);
	string part2 = line.substr(pos + 1);
	CDate date = CDate::FromString(part1);
	pair_t pair(date, line.c_str()+pos+1);
	return pair;
}

uint64_t SelectSortingValue(pair_t& pair)
{
	return pair.GetKey().GetInternalValue();
}

int main()
{
	// read lines from console
	vector<string> lines;
	string line;
	while (getline(cin, line))
	{
		if (!line.empty())
			lines.push_back(line);
	}

	// parse lines to pairs
	size_t size = lines.size();
	pair_t* array = new pair_t[size];
	for (size_t i = 0; i < size; i++)
	{
		pair_t pair = ParseLine(lines[i]);
		array[i] = pair;
	}

	// sorting
	CSorter::RadixSort(array, size, SelectSortingValue);

	// output results
	for (size_t i = 0; i < size; i++)
	{
		pair_t pair = array[i];
		cout << pair.GetKey().GetInputString() << "\t" << pair.GetValue() << endl;
	}
}
