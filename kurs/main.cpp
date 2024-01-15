// Дискретный анализ, Курсовая работа, Диана Колпакова
// "Наивный Байесовкий классификатор"

#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;


// --- StringUtils ----------------------------------------------------------------------------

class StringUtils
{
public:
	static vector<string> split(const string& text, const string& delimeter, const bool keepEmpty = false);
	static string toLower(const string& text);
	static string remove(const string& text, const vector<char>& chars);
};

// разделить строку на отдельные подстроки по разделителю с сохранением или без сохранения пустых под-строк
vector<string> StringUtils::split(const string& text, const string& delimeter, const bool keepEmpty)
{
	vector<string> result;
	string temp = text;
	while (true)
	{
		size_t pos = temp.find(delimeter);
		if (pos == string::npos)
		{
			if (keepEmpty || temp.length() > 0)
				result.push_back(temp);
			break;
		}
		string part1 = temp.substr(0, pos);
		string part2 = temp.substr(pos + 1);
		if (keepEmpty || part1.length() > 0)
			result.push_back(part1);
		temp = part2;
	}
	return result;
}

// перевсти строку в нижний регистр
string StringUtils::toLower(const string& text)
{
	string temp = text;
	for (int i = 0; i < temp.length(); i++)
		temp[i] = tolower(temp[i]);
	return temp;
}

// удалить из строки все указанные символы
string StringUtils::remove(const string& text, const vector<char>& chars)
{
	//string temp = text;
	//for (int i = 0; i < chars.size(); i++)
	//	erase(temp, chars[i]);
	//return temp;
	vector<char> result;
	for (int i = 0; i < text.length(); i++)
	{
		if (find(chars.begin(), chars.end(), text[i]) == chars.end())
			result.push_back(text[i]);
	}
	return string(result.begin(), result.end());
}

// --- StringCollectionUtils ------------------------------------------------------------------

class StringCollectionUtils
{
public:
	static vector<string> remove(const vector<string>& texts, const vector<string>& items);
};

// удалить из коллекции строк все указанные строки
vector<string> StringCollectionUtils::remove(const vector<string>& texts, const vector<string>& items)
{
	vector<string> result;
	for (int i = 0; i < texts.size(); i++)
	{
		if (find(items.begin(), items.end(), texts[i]) == items.end())
			result.push_back(texts[i]);
	}
	return result;
}

// --- Brain ----------------------------------------------------------------------------------

class Brain
{
private:
	// параметры класса
	int _klassCount;										// количество классов

	// внутренние данные
	map<string, vector<int>> _wordInKlassCounts;			// колиечство вхождений определенного слова в определенный класс
	vector<int> _totalWordPerKlassCount;					// количество слов в определенном классе
	int _totalWordCount;									// общее количество слов

public:
	Brain();
	Brain(int klassCount);
	virtual ~Brain();

	// добавить определенное слово в определенный класс
	void add(string word, int klass);

	// подсчитать вероятность нахождения определенного слова в определенном класск
	double calcP(string word, int klass);
};

Brain::Brain()
{
	_klassCount = 0;
}

Brain::Brain(int klassCount)
{
	// параметры класса
	_klassCount = klassCount;

	// инициализация класса
	_totalWordPerKlassCount = vector<int>(klassCount);
	_totalWordCount = 0;
}

Brain::~Brain()
{
}

// добавить определенное слово в определенный класс
void Brain::add(string word, int klass)
{
	//if (klass >= _klassCount)
	//	throw new Exception("Invalid klass");

	if (_wordInKlassCounts.find(word) == _wordInKlassCounts.end())		// если такого слова еще не было
	{
		_wordInKlassCounts[word] = vector<int>(_klassCount);			// добавить такое слово с нулевыми счетчиками количества этого слова в классах
		_totalWordCount++;												// увеличить счетчик слов
	}
	_wordInKlassCounts[word][klass]++;									// увеличить счетчик определенного слова в определенном классе
	_totalWordPerKlassCount[klass]++;									// увеличить счетчик всех слов в определенном классе
}

// подсчитать вероятность нахождения определенного слова в определенном класск
double Brain::calcP(string word, int klass)
{
	//if (klass >= _klassCount)
	//	throw new Exception("Invalid klass");

	vector<int> wordPerKlassCount(_klassCount);							// подготовим нулевые счетчики количества определенного слова в классах
	if (_wordInKlassCounts.find(word) != _wordInKlassCounts.end())		// если такое слово уже есть
		wordPerKlassCount = _wordInKlassCounts[word];					// то заменим нулевые счетчики на найденные счетчики этого слова

	double alpha = 1.0;													// некий параметр склаживания
	int wordOfKlassCount = wordPerKlassCount[klass];
	int totalWordOflassCount = _totalWordPerKlassCount[klass];
																		// вычислене вероятности
	double p = (alpha + (double)wordOfKlassCount) / (alpha * (double)_totalWordCount + (double)totalWordOflassCount);
	return p;
}

// --- NaiveBayesClassifier -------------------------------------------------------------------

class NaiveBayesClassifier
{
private:
	// параметры класса
	int _klassCount;										// количество классов
	vector<char> _punctuationChars;							// знаки припинания. Из текстов будут исключаться эти знаки
	vector<string> _stopWords;								// мусорные слова. Из текстов будут исключаться эти слова

	// внутренние данные
	Brain _brain;
	vector<int> _docPerKlassCount;							// количество текстов в определенных классах
	int _totalDocCount;										// общее количество текстов

public:
	NaiveBayesClassifier(int klassCount, const vector<char>& punctuationChars, const vector<string>& stopWords);
	virtual ~NaiveBayesClassifier();

	// натренировать классификатор на определенном тексте и классе
	void train(const int klass, const string& text);

	// спросить классификатор насчет класса определенного текста
	int query(const string& text);

private:
	vector<string> parse(const string& text);
};

NaiveBayesClassifier::NaiveBayesClassifier(int klassCount, const vector<char>& punctuationChars, const vector<string>& stopWords)
{
	// параметры класса
	_klassCount = klassCount;
	_punctuationChars = punctuationChars;
	_stopWords = stopWords;
	// инициализация класса
	_brain = Brain(klassCount);
	_totalDocCount = 0;
	_docPerKlassCount = vector<int>(klassCount);
}

NaiveBayesClassifier::~NaiveBayesClassifier()
{
}

// натренировать классификатор на определенном тексте и классе
void NaiveBayesClassifier::train(const int klass, const string& text)
{
	vector<string> words = parse(text);				// разобрать тренировочный текст на отдельные слова в нижнем регистре, без пунктуации и без мусорных слов

	for (int i = 0; i < words.size(); i++)
		_brain.add(words[i], klass);				// добавить все слова в мозг

	_totalDocCount++;								// наводим статистику - сколько документов всего
	_docPerKlassCount[klass]++;						// и сколько документов определенного класса
}

// спросить классификатор насчет класса определенного текста
int NaiveBayesClassifier::query(const string& text)
{
	vector<string> words = parse(text);				// разобрать тестовый текст на отдельные слова в нижнем регистре, без пунктуации и без мусорных слов
	vector<double> pp(_klassCount);					// вероятность того, что документ принадлежит определенному классу

	// подсчитать вероятности того, что документ принадлежит определенному классу
	for (int klass = 0; klass < _klassCount; klass++)		// попробовать все возможные классы
	{
		pp[klass] = (double)_docPerKlassCount[klass] / (double)_totalDocCount;  // учитываем коэфициент - вес документов определенного класса 
		for (int i = 0; i < words.size(); i++)				// перебрать все слова из тестового текста
		{
			double p = _brain.calcP(words[i], klass);		// попросить мозг подсчитать вероятность слова в определенном классе
			pp[klass] *= p;									// учесть эту вероятность
		}
	}

	// узнать веротяность какого класса оказалась выше
	int maxPKlass = 0;		// изначально считать максимальной вероятность класс 0
	for (int klass = 1; klass < _klassCount; klass++)		// перебрать классы начиная с 1 и далее
	{
		if (pp[klass] > pp[maxPKlass])		// если вероятность очередного класса стала выше
			maxPKlass = klass;				// то этот класс и считать максимальным
	}
	return maxPKlass;						// вернуть номер класса
}

// разбить заданный текст на отдельные слова, убрав знаки препинания и мусорные слова
vector<string> NaiveBayesClassifier::parse(const string& text)
{
	string textWoPunctuation = StringUtils::remove(text, _punctuationChars);				// убрать из текста знаки препинания
	string textinLowerCase = StringUtils::toLower(textWoPunctuation);						// перевести в нижний регистр
	vector<string> allWords = StringUtils::split(textinLowerCase, " ");						// разбить на отдлельные слова
	vector<string> wordsWoStopWords = StringCollectionUtils::remove(allWords, _stopWords);	// убрать мусорные слова
	return wordsWoStopWords;
}

// --- MAIN -----------------------------------------------------------------------------------

int main()
{
	int klassCount = 2;
	vector<char> punctuationChars = { '.', '!', '?' };
	vector<string> stopWords;				// = { "and", "are", "my" };
	NaiveBayesClassifier classifier(klassCount, punctuationChars, stopWords);

	int trainingCaseCount = 0;
	int testCaseCount = 0;
	cin >> trainingCaseCount;					// ввести количество тренировок
	cin >> testCaseCount;						// ввести количнство тестов

	for (int i = 0; i < trainingCaseCount; i++)
	{
		int klass;
		string text;
		cin >> klass;							// ввести класс текста тренировки
		cin.get();
		getline(cin, text);						// ввести сам текст тренировки
		classifier.train(klass, text);			// натренировать классификатор на тексте и его классе
	}

	for (int i = 0; i < testCaseCount; i++)
	{
		string text;
		getline(cin, text);						// ввести сам текст теста
		int klass = classifier.query(text);		// спросить классификатор насчтет текста
		cout << klass << endl;					// вывести класс текст
	}
}
