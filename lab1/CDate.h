#pragma once
#include  <cstdint>
#include  <string>

using namespace std;

class CDate
{
private:
	uint16_t _year;
	uint16_t _month;
	uint16_t _day = 0;
	string _inputString;
	string _formattedString;
	uint64_t _internalValue;

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

