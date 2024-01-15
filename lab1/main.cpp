#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
//#include "TVector.h"
#include "TPair.h"
#include "CDate.h"
#include "CSorter.h"

using namespace std;

TPair<CDate, string> ParseLine(const string& line)
{
	size_t pos = line.find('\t');
	if (pos == string::npos)
		throw runtime_error("main, exception: not tab delimiter in input line");
	string part1 = line.substr(0, pos);
	string part2 = line.substr(pos + 1);
	CDate date = CDate::FromString(part1);
	TPair<CDate, string> pair(date, part2);
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
		cout << pair.GetKey().GetFormattedString() << "\t" << pair.GetValue() << endl;
	}
}
