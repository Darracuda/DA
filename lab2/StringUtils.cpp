#include "StringUtils.h"

using namespace std;

vector<string> StringUtils::split(const string& str, const string& delimeter)
{
	vector<string> result;
	string temp = str;

	while (true)
	{
		size_t pos = temp.find(delimeter);
		if (pos == string::npos)
		{
			result.push_back(temp);
			break;
		}
		string part1 = temp.substr(0, pos);
		string part2 = temp.substr(pos + 1);
		result.push_back(part1);
		temp = part2;
	}

	return result;
}

int StringUtils::compareIgnoringCase(const string& str1, const string& str2)
{
	size_t length1 = str1.length();
	size_t length2 = str2.length();
	size_t minLength = (length1 < length2) ? length1 : length2; // min

	for (size_t i = 0; i < minLength; i++)
	{
		char ch1 = tolower(str1[i]);
		char ch2 = tolower(str2[i]);
		if (ch1 < ch2)
			return -1;
		if (ch1 > ch2)
			return +1;
	}
	if (length1 < length2)
		return -1;
	if (length1 > length2)
		return +1;
	return 0;
}

bool StringUtils::equalIgnoringCase(const string& str1, const string& str2)
{
	return StringUtils::compareIgnoringCase(str1, str2) == 0;
}

bool StringUtils::startsWith(const string& str1, const string& str2)
{
	size_t length1 = str1.length();
	size_t length2 = str2.length();
	if (length2 > length1)
		return false;

	for (size_t i = 0; i < length2; i++)
	{
		char ch1 = tolower(str1[i]);
		char ch2 = tolower(str2[i]);
		if (ch1 != ch2)
			return false;
	}

	return true;
}
