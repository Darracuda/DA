// DA, Lab 2, Diana Kolpakova
// variant 2, RB tree

#include <iostream>
#include <string>
#include <vector>

#include "RedBlackTree.h"
#include "StringUtils.h"

using namespace std;

RedBlackTree* tree;

// execute commands with parameteres
void execute(const string& str)
{
	try
	{
		vector<string> result = StringUtils::split(str, " ");

		if (result.size() == 3 && result[0] == "+")
		{
			string key = result[1];
			uint64_t value = stoull(result[2].c_str());
			bool added = tree->add(key, value);
			if (added)
				cout << "OK" << endl;
			else
				cout << "Exist" << endl;
		}
		else if (result.size() == 2 && result[0] == "-")
		{
			string key = result[1];
			bool found = tree->remove(key);
			if (found)
				cout << "OK" << endl;
			else
				cout << "NoSuchWord" << endl;
		}
		else if (result.size() == 2 && result[0] == "?")
		{
			string key = result[1];
			uint64_t value = 0;
			bool found = tree->get(key, &value);
			if (found)
				cout << "OK:" << value << endl;
			else
				cout << "NoSuchWord" << endl;
		}
		else if (result.size() == 1)
		{
			string key = result[0];
			uint64_t value = 0;
			bool found = tree->get(key, &value);
			if (found)
				cout << "OK:" << value << endl;
			else
				cout << "NoSuchWord" << endl;
		}
		else if (result.size() == 3 && result[0] == "!" && result[1] == "Save")
		{
			string filename = result[2];
			tree->saveToFile(filename);
			cout << "OK" << endl;
		}
		else if (result.size() == 3 && result[0] == "!" && result[1] == "Load")
		{
			string filename = result[2];
			RedBlackTree* tree2 = RedBlackTree::createFromFile(filename);
			if (tree != nullptr)
				delete tree;
			tree = tree2;
			cout << "OK" << endl;
		}
		else
		{
			throw runtime_error("invalid command or parameters");
		}
	}
	catch (exception ex)
	{
		cout << "ERROR: " << ex.what() << endl;
	}
}

// main function
int main()
{
	tree = new RedBlackTree();

	string line;
	while (getline(cin, line))
	{
		execute(line);
	}

	if (tree != nullptr)
		delete tree;

	return 0;
}
