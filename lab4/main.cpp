// DA, Lab 4, Diana Kolpakova
// variant 4-1
// 4. Поиск одного образца основанный на построении Z-блоков.
// 1. Слова не более 16 знаков латинского алфавита (регистронезависимые)

// aux info:
// https://ru.algorithmica.org/cs/string-searching/z-function/
// https://habr.com/ru/post/113266/

#include <iostream>
#include <string>
#include <vector>

using namespace std;

//===================================================================================================

class StringUtils
{
public:
	// splitting the input string by the delimeter and return array of sub-strings
	static vector<string> split(const string& str, const string& delimeter, const bool skipEmpty = true);

	// compare two strings
	static int compare(const string& str1, const string& str2, const bool ignoreCase = true);
	static bool equal(const string& str1, const string& str2, const bool ignoreCase = true);

	// check id a string starts with another
	static bool startsWith(const string& str1, const string& str2, const bool ignoreCase = true);
};

vector<string> StringUtils::split(const string& str, const string& delimeter, const bool skipEmpty)
{
	vector<string> result;
	string temp = str;

	while (true)
	{
		size_t pos = temp.find(delimeter);
		if (pos == string::npos)
		{
			if (skipEmpty && temp.length() == 0)
			{ /* nothing */ }
			else
				result.push_back(temp);

			break;
		}
		string part1 = temp.substr(0, pos);
		string part2 = temp.substr(pos + 1);

		if (skipEmpty && part1.length() == 0)
		{ /* nothing */ }
		else
			result.push_back(part1);

		temp = part2;
	}

	return result;
}

int StringUtils::compare(const string& str1, const string& str2, const bool ignoreCase)
{
	size_t length1 = str1.length();
	size_t length2 = str2.length();
	size_t minLength = (length1 < length2) ? length1 : length2; // min

	for (size_t i = 0; i < minLength; i++)
	{
		char ch1 = str1[i];
		char ch2 = str2[i];
		if (ignoreCase)
		{
			ch1 = tolower(str1[i]);
			ch2 = tolower(str2[i]);
		}
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

bool StringUtils::equal(const string& str1, const string& str2, const bool ignoreCase)
{
	return StringUtils::compare(str1, str2, ignoreCase) == 0;
}

bool StringUtils::startsWith(const string& str1, const string& str2, const bool ignoreCase)
{
	size_t length1 = str1.length();
	size_t length2 = str2.length();
	if (length2 > length1)
		return false;

	for (size_t i = 0; i < length2; i++)
	{
		char ch1 = str1[i];
		char ch2 = str2[i];
		if (ignoreCase)
		{
			ch1 = tolower(str1[i]);
			ch2 = tolower(str2[i]);
		}
		if (ch1 != ch2)
			return false;
	}

	return true;
}

//===================================================================================================
class WordInfo
{
private:
	string _word;
	size_t _lineNo;
	size_t _wordNo;
	static WordInfo _sentinel;

public:
	WordInfo(const string& word, const size_t lineNo, const size_t wordNo);
	string& getWord();
	size_t getLineNo();
	size_t getWordNo();

	static WordInfo& getSentinel();

	static vector<WordInfo> readFromConsole();
};

WordInfo::WordInfo(const string& word, const size_t lineNo, const size_t wordNo)
{
	_word = word;
	_lineNo = lineNo;
	_wordNo = wordNo;
}

string& WordInfo::getWord()
{
	return _word;
}

size_t WordInfo::getLineNo()
{
	return _lineNo;
}

size_t WordInfo::getWordNo()
{
	return _wordNo;
}

WordInfo WordInfo::_sentinel("<*$$ SENTINEL $$*>", 0, 0);

WordInfo& WordInfo::getSentinel()
{
	return _sentinel;
}

vector<WordInfo> WordInfo::readFromConsole()
{
	vector<WordInfo> results;

	string line;				// промежуточная строка
	for (size_t lineNo = 0; getline(cin, line); lineNo++)
	{
		vector<string> parts = StringUtils::split(line, " ", true);
		for (size_t wordNo = 0; wordNo < parts.size(); wordNo++)
		{
			string word = parts[wordNo];
			WordInfo info(word, lineNo, wordNo+1);  // <-!!!
			results.push_back(info);
		}
	}

	return results;
}

//===================================================================================================

class ZBlock
{
public:
	static vector<size_t> slow_z_function(const vector<WordInfo>& input);
	static vector<size_t> z_function(const vector<WordInfo>& input);

	static vector<size_t> find(const vector<WordInfo>& what_data, const vector<WordInfo>& where_data);
};

vector<size_t> ZBlock::slow_z_function(const vector<WordInfo>& input)
{
	size_t len = input.size();
	vector<size_t> z(len, 0); // z[0] считается не определенным

	for (size_t i = 1; i < len; i++)
	{
		// если мы не вышли за границу и следующие символы совпадают
		while ((i + z[i] < len))
		{
			WordInfo wi1 = input[z[i]];
			WordInfo wi2 = input[i+z[i]];
			if (!StringUtils::equal(wi1.getWord(), wi2.getWord(), true))
				break;
			z[i]++;
		}
	}

	return z;
}

vector<size_t> ZBlock::z_function(const vector<WordInfo>& input)
{
	size_t len = input.size();
	vector<size_t> z(len, 0);
	size_t leftpos = 0, rightpos = 0;

	for (size_t i = 1; i < len; i++)
	{
		// если мы уже видели этот символ
		if (i <= rightpos)
			// то мы можем попробовать его инициализировать z[i - l],
			// но не дальше правой границы: там мы уже ничего не знаем
			z[i] = min(rightpos - i + 1, z[i - leftpos]);

		// дальше каждое успешное увеличение z[i] сдвинет z-блок на единицу
		while ((i + z[i] < len))
		{
			WordInfo wi1 = input[z[i]];
			WordInfo wi2 = input[i + z[i]];
			if (!StringUtils::equal(wi1.getWord(), wi2.getWord(), true))
				break;
			z[i]++;
		}
		// проверим, правее ли мы текущего z-блока
		if (i + z[i] - 1 > rightpos)
		{
			rightpos = i + z[i] - 1;
			leftpos = i;
		}
	}
	return z;
}

vector<size_t> ZBlock::find(const vector<WordInfo>& what_data, const vector<WordInfo>& where_data)
{
	// склейка данных через сентинель
	vector<WordInfo> all_data_and_sentinel;
	for (size_t i = 0; i < what_data.size(); i++)
		all_data_and_sentinel.push_back(what_data[i]);
	all_data_and_sentinel.push_back(WordInfo::getSentinel());
	for (size_t i = 0; i < where_data.size(); i++)
		all_data_and_sentinel.push_back(where_data[i]);

	vector<size_t> results;
	vector<size_t> z = ZBlock::z_function(all_data_and_sentinel);

	for (size_t i = 0; i < where_data.size(); i++)		// для всех позиций данных, в которых ищем
	{
		size_t j = i + what_data.size() + 1;	// вычисляем позицию в z-значениях для позиции i данных, в которых ищем

		if (z[j] == what_data.size())			// если очередное z-значение равно длине данных, которые ищем
			results.push_back(i);
	}
	return results;
}


int main()
{
	vector<WordInfo> all_data = WordInfo::readFromConsole();
	vector<WordInfo> what_data;		// данные, которые будем искать
	vector<WordInfo> where_data;	// данные, где будем искать
	for (size_t i = 0; i < all_data.size(); i++)
	{
		WordInfo wi = all_data[i];
		if (wi.getLineNo() == 0)
			what_data.push_back(wi);
		else
			where_data.push_back(wi);
	}

	vector<size_t> positions = ZBlock::find(what_data, where_data);   // получим позиции найденных фрагментов строк

	for (size_t i = 0; i < positions.size(); i++)
	{
		WordInfo wi = where_data[positions[i]];
		cout << wi.getLineNo() << ", " << wi.getWordNo() << endl;
	}
}
