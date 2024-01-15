#include <stdexcept>
#include <format>
#include "CDate.h"

using namespace std;

CDate::CDate(const CDate& other)
{
	_year = other._year;
	_month = other._month;
	_day = other._day;
	_inputString = other._inputString;
	_formattedString = other._formattedString;
	_internalValue = other._internalValue;
}

CDate::CDate(const uint16_t& year, const uint16_t& month, const uint16_t& day, const string& inputString)
{
	_year = year;
	_month = month;
	_day = day;

	_inputString = inputString;

	// formatting of input string
	char buffer[100];
	::snprintf(buffer, 100, "%02d.%02d.%04d", day, month, year);
	_formattedString = string(buffer);

	// example 20220224 for 24.02.2022
	_internalValue = (uint64_t)day + 100 * ((uint64_t)month + 100 * (uint64_t)year);  
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
	::sscanf_s(input.c_str(), "%d.%d.%d", &day, &month, &year);

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

string CDate::GetFormattedString()
{
	return _formattedString;
}

uint64_t CDate::GetInternalValue()
{
	return _internalValue;
}
