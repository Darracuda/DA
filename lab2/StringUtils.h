#pragma once

#include <string>
#include <vector>

using namespace std;

class StringUtils
{
public:
	// splitting the input string by the delimeter and return array of sub-strings
	static vector<string> split(const string& str, const string& delimeter);

	// compare two strings ignoring case
	static int compareIgnoringCase(const string& str1, const string& str2);
	static bool equalIgnoringCase(const string& str1, const string& str2);

	static bool startsWith(const string& str1, const string& str2);
};

