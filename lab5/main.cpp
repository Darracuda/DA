// Суффиксные деревья
// Вариант 1: Поиск в известном тексте неизвестных
//заранее образцов
//Найти в заранее известном тексте поступающие на вход образцы.
//Входные данные : текст располагается на первой строке, затем, до конца
//файла, следуют строки с образцами.
//Выходные данные : для каждого образца, найденного в тексте, нужно
//распечатать строчку, начинающуюся с последовательного номера этого
//образца и двоеточия, за которым, через запятую, нужно перечислить
//номера позиций, где встречается образец в порядке возрастания.
//Входной файл Выходной файл
//abcdabc
//abcd
//bcd
//bc
//
//1: 1
//2 : 2
//3 : 2, 6

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

//--- SUFFIX TREE NODE --------------------------------------

class Node
{
private:
	int _suffixNo;								// номер суффикса для конечной ноды и -1 для промежуточной ноды
	int _leftBound, _rightBound;				// индекс начала и индекс конца подстроки на ноде

	Node* _parent;
	map<char, Node*> _children;					// ссылки по дочерние ноды
	Node* _suffixLink;							// суффиксная ссылка

public:
	Node();
	Node(Node* parent, const int suffixNo, const int leftBound, const int rightBound);
	virtual ~Node();

	bool isLeaf();								// является ли узел листом (это когда нет дочерних узлов)
	Node* getChild(const char ch);				// взять дочерний узел для символа из алфавита. возвращает nullptr если для указанного символа нет дочернего узла
	void addChild(const char ch, Node* node);	// добавить дочерний узел для символа из алфавита
	int getLeftBound();							// начальная позиция ...
	int getRightBound();						// и конечная позиция подстроки-суффикса по отношению к строке, по которой построено дерево
	int getSuffixNo();							// получить номер суффикса по отношению к строке, по которой построено дерево. 0 - сама строка, 1 - строк без первого символа, и т.д.
	vector<Node*> getLeaves();					// получить список листовых узлов с этого узла. если этот узел сам является листом, то возвращает самого себя
	void split(const char ch, const int index); // разделить текущий узел на два связанных узла, т.е. у этого узла появится дочерний узел с соответвенной передачей своих дочерних узлов

	string toJson(const string& text);
};

Node::Node() : Node(nullptr, -1, 0, -1)
{
}

Node::Node(Node* parent, const int suffixNo, const int leftBound, const int rightBound)
{
	_parent = parent;
	_suffixNo = suffixNo;
	_leftBound = leftBound;
	_rightBound = rightBound;
	_suffixLink = nullptr;
}

Node::~Node()
{
	for (auto const& item : _children)
	{
		delete item.second;
	}
}

// является ли узел листом (это когда нет дочерних узлов)
bool Node::isLeaf()
{
	return (_children.size() == 0);
}

// взять дочерний узел для символа из алфавита. возвращает nullptr если для указанного символа нет дочернего узла
Node* Node::getChild(const char ch)
{
	if (_children.find(ch) == _children.end())
		return nullptr;
	return _children[ch];
}

// добавить дочерний узел для символа из алфавита
void Node::addChild(const char ch, Node* node)
{
	_children[ch] = node;
	_suffixNo = -1;
}

// начальная позиция подстроки-суффикса по отношению к строке, по которой построено дерево
int Node::getLeftBound()
{
	return _leftBound;
}

// конечная позиция подстроки-суффикса по отношению к строке, по которой построено дерево
int Node::getRightBound()
{
	return _rightBound;
}

// получить номер суффикса по отношению к строке, по которой построено дерево. 0 - сама строка, 1 - строк без первого символа, и т.д.
int Node::getSuffixNo()
{
	return _suffixNo;
}

// получить список листовых узлов с этого узла. если этот узел сам является листом, то возвращает самого себя
vector<Node*> Node::getLeaves()
{
	vector<Node*> result;
	if (isLeaf())
	{
		result.push_back(this);					// если сам лист, то добавить только себя в наш список листов
	}
	else
	{
		for (auto const& item : _children)		// если не лист, то пройтись по всем дочерних узлам
		{
			vector<Node*> leaves = item.second->getLeaves();			// узнать у дочернего узла его список листов
			result.insert(result.end(), leaves.begin(), leaves.end());		// добавить его листы в наш список листов
			//for (size_t i = 0; i < leaves.size(); i++)
			//{
			//	result.push_back(leaves[i]);
			//}
		}
	}
	return result;
}

// разделить текущий узел на два связанных узла, т.е. у этого узла появится дочерний узел с соответвенной передачей своих дочерних узлов
void Node::split(const char ch, const int index)
{
	Node* node = new Node(this, _suffixNo, index, _rightBound);
	_rightBound = index - 1;
	node->_children = _children;
	_children.clear();
	addChild(ch, node);
}


string Node::toJson(const string& text)
{
	string part = text.substr(_leftBound, _rightBound - _leftBound + 1);
	string result = "{\"text\":\"" + part + "\"";
	result += ",\"left\":" + to_string(_leftBound);
	result += ",\"right\":" + to_string(_rightBound);

	if (_suffixNo >= 0)
	{
		result += ",\"no\":" + to_string(_suffixNo);
	}

	if (_children.size() > 0)
	{
		result += ",\"children\":[";
		int i = 0;
		for (auto const& item : _children)
		{
			if (i != 0)
				result += ",";
			result += item.second->toJson(text);
			i++;
		}
		result += "]";
	}

	result += "}";
	return result;
}


//--- SUFFIX TREE -------------------------------------------

class SuffixTree
{
private:
	const char _terminal = '$';
	string _text;
	Node* _root;

public:
	SuffixTree(const string& text);					// построить дерево
	virtual ~SuffixTree();							// уничтожить дерево

	vector<int> find(const string& text);			// найти подстроку в дереве

	string toJson();

private:
	void _addSuffix(const int suffixNo);			// добавить очередной суффикс
};

// построить дерево
SuffixTree::SuffixTree(const string& text)
{
	_text = text + _terminal;					// сразу в конец добавляем терминальный символ (не из алфавита)
	_root = new Node();							// корневой узел

	for (int i = 0; i < _text.length(); i++)	// переберем все суффиксы (в том числе и саму исходную строку)
		_addSuffix(i);								// добавить очередной суффикс, т.е. подстроку с i
}

// уничтожить дерево
SuffixTree::~SuffixTree()
{
	delete _root;
}

// найти подстроку в дереве
vector<int> SuffixTree::find(const string& text)
{
	vector<int> result;

	Node* current = _root;								// начнем с корневого узла
	int currentleftBound = current->getLeftBound();		// начало строчки в узле
	int currentRightBound = current->getRightBound();	// конеч строчки в узле
	int currentIndex = currentleftBound;				// положение очередного символа в строчке на узле
	char currentChar = _text[currentIndex];				// сам очередной символ

	for (int i = 0; i < text.length(); i++)					// переберем все позиции символов в поисковой подстроке
	{
		char ch = text[i];										// очередной символ в поисковой подстроке
		if (currentIndex > currentRightBound)				// если на узле больше нет символов
		{
			Node* child = current->getChild(ch);			// пробуем найти дочерний узел для символа поисковой строки
			if (child == nullptr)							// если такого дочернего узла нет, ...
				return result;								// то подстрока не найдена в дереве и возврат пустого результата
			current = child;								// найденный дочерний узел сделаем текущим узлом
			currentleftBound = current->getLeftBound();		// начало строчки в узле
			currentRightBound = current->getRightBound();	// конец строчки в узле
			currentIndex = currentleftBound;				// положение очередного символа в строчке на узле
			currentChar = _text[currentIndex];				// сам очередном символ
		}

		if (ch != currentChar)								// если символы из поисковой подстроки и строчки на узле не совпали, ...
			return result;									// то подстрока не найдена в длереве и возврат пустого результата

		currentIndex++;										// следующаяя положение очередного символа в строчке на узле 
		currentChar = _text[currentIndex];					// сам очередной символ
	}

	vector<Node*> leaves = current->getLeaves();			// всю поисковую строку прошли, все найдено, остановилисть на каком-то узле, и с него возьмем листы
	for (int i = 0; i < leaves.size(); i++)
		result.push_back(leaves[i]->getSuffixNo());			// переложим номера суффиксов с листов в отдельный массив
	sort(result.begin(), result.end());						// и отсортируем его
	return result;											// и вернем
}

// добавление очередного суффикса в дерево
void SuffixTree::_addSuffix(const int suffixNo)
{
	Node* current = _root;								// начнем с корневого узла
	int currentleftBound = current->getLeftBound();		// начало строчки в узле
	int currentRightBound = current->getRightBound();	// конеч строчки в узле
	int currentIndex = currentleftBound;				// положение очередного символа в строчке на узле
	char currentChar = _text[currentIndex];				// сам очередной символ
	char ch;
	int i;

	for (i = suffixNo; i < _text.length(); i++)		// переберем все позиции символов в строке-суффиксе
	{
		ch = _text[i];										// очередной символ в поисковой подстроке
		if (currentIndex > currentRightBound)				// если на узле больше нет символов
		{
			Node* child = current->getChild(ch);			// пробуем найти дочерний узел для символа поисковой строки
			if (child == nullptr)							// если такого дочернего узла нет, ...
				break;										// то хватит бегать, выходим из цикла
			current = child;								// найденный дочерний узел сделаем текущим узлом
			currentleftBound = current->getLeftBound();		// начало строчки в узле
			currentRightBound = current->getRightBound();	// конец строчки в узле
			currentIndex = currentleftBound;				// положение очередного символа в строчке на узле
			currentChar = _text[currentIndex];				// сам очередном символ
		}

		if (ch != currentChar)								// если символы из строки-суффикса и строчки на узле не совпали, ...
		{
			current->split(currentChar, currentIndex);		// то разделить текущую ветку на 2 ветки, и хватит бегать, выходим из цкила
			break;
		}

		currentIndex++;										// следующаяя положение очередного символа в строчке на узле 
		currentChar = _text[currentIndex];					// сам очередной символ
	}

	Node* node = new Node(current, suffixNo, i, (int)_text.length() - 1);	// создадим новый узел для остатков строки-суффикса
	current->addChild(ch, node);											// добавим узел к текущему узлу
}

string SuffixTree::toJson()
{
	string result = "{\"text\":\"" + _text + "\",\"root\":" + _root->toJson(_text) + "}";
	return result;
}


//--- MAIN --------------------------------------------------

int main()
{
	string text;
	string str;

	getline(cin, text);                     // ввод строки, на основании которой будет построено суффиксное дерево
	SuffixTree tree(text);					// построение дерева

	string json = tree.toJson();

	int number = 0;							// номер поисковой строчки
	while (getline(cin, str))               // читаем все оставшиеся строки
	{
		vector<int> indices = tree.find(str);				// и находим эту строчку в суффиксном дереве
		if (indices.size() > 0)
		{
			cout << (number + 1) << ": ";				// перед первой позицией выведем номер поисковой строчки (нумерация с 1)
			for (int i = 0; i < indices.size(); i++)			// для каждой позиции найденной строчки
			{
				if (i != 0)
					cout << ", ";								// перед остальными позициями выведем разделитель-запятую
				cout << (indices[i] + 1);						// выведем позицию найденной строчки (нумерация с 1)
			}
			cout << endl;
		}
		number++;											// номер поисковой строчки
	}
	return 0;
}
