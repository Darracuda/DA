#include <stdexcept>
#include <fstream>

#include "RedBlackTree.h"
#include "StringUtils.h"

using namespace std;

RedBlackTree::RedBlackTree()
{
	// any key & value - it doesn't matter, but color=BLACK does matter
	_nilNode = new RedBlackTreeNode("<NIL>", 0, RedBlackTreeNode::Color::Black, nullptr, nullptr, nullptr);

	_rootNode = _nilNode;
}

RedBlackTree::~RedBlackTree()
{
	clearTree();
	delete _nilNode;
}

bool RedBlackTree::add(const string& key, const uint64_t value)
{
	RedBlackTreeNode* foundNode = findNode(key);
	if (foundNode != _nilNode)		// already exists
		return false;

	RedBlackTreeNode* newNode = new RedBlackTreeNode(key, value, RedBlackTreeNode::Color::Red,
		_nilNode, _nilNode, _nilNode);
	tkRBInsert(newNode);

	return true;
}

bool RedBlackTree::remove(const string& key)
{
	RedBlackTreeNode* foundNode = findNode(key);
	if (foundNode == _nilNode)		// not found
		return false;

	RedBlackTreeNode* deletedNode = tkRBDelete(foundNode);
	delete deletedNode;

	return true;
}

bool RedBlackTree::get(const string& key, uint64_t* pValue)
{
	RedBlackTreeNode* foundNode = findNode(key);
	if (foundNode == _nilNode)		// not found
		return false;

	*pValue = foundNode->getValue();

	return true;
}

void RedBlackTree::saveToFile(const string& filename)
{
	if (StringUtils::startsWith(filename, "/"))
		throw runtime_error("Couldn't create file");

	ofstream stream;
	stream.open(filename);

	saveSubTree(_rootNode, stream);

	stream.close();
}

RedBlackTree* RedBlackTree::createFromFile(const string& filename)
{
	if (StringUtils::startsWith(filename, "/"))
		throw runtime_error("Couldn't open file");

	ifstream stream;
	stream.open(filename);

	RedBlackTree* tree = new RedBlackTree();
	string key;
	uint64_t value;
	while (stream >> key >> value)
	{
		tree->add(key, value);
	}

	stream.close();

	return tree;
}



RedBlackTreeNode* RedBlackTree::findNode(const string& key)
{
	RedBlackTreeNode* currentNode = _rootNode;
	while (currentNode != _nilNode)
	{
		int result = StringUtils::compareIgnoringCase(key, currentNode->getKey());
		if (result < 0)
			currentNode = currentNode->getLeftNode();		// go left
		else if (result > 0)
			currentNode = currentNode->getRightNode();		// go right
		else
			return currentNode;		// found
	}
	return _nilNode;		// not found
}

RedBlackTreeNode* RedBlackTree::tkTreeMinimum(RedBlackTreeNode* x)
{
	if (x == nullptr || x == _nilNode)
		throw runtime_error("null or nil argument in tkTreeMinimum");
	while (x->getLeftNode() != _nilNode)	//*1
	{
		x = x->getLeftNode();				//*2
	}
	return x;								//*3
}

RedBlackTreeNode* RedBlackTree::tkTreeMaximum(RedBlackTreeNode* x)
{
	if (x == nullptr || x == _nilNode)
		throw runtime_error("null or nil argument in tkTreeMaximum");
	while (x->getRightNode() != _nilNode)	//*1
	{
		x = x->getRightNode();				//*2
	}
	return x;								//*3
}

RedBlackTreeNode* RedBlackTree::tkTreeSuccessor(RedBlackTreeNode* x)
{
	if (x == nullptr || x == _nilNode)
		throw runtime_error("null or nil argument in tkTreeSuccessor");

	if (x->getRightNode() != _nilNode)				//*1
		return tkTreeMinimum(x->getRightNode());	//*2

	RedBlackTreeNode* y = x->getParentNode();		//*3
	while (y != _nilNode && x == y->getRightNode())	//*4
	{
		x = y;										//*5
		y = y->getParentNode();						//*6
	}
	return y;										//*7
}

RedBlackTreeNode* RedBlackTree::tkTreePredecessor(RedBlackTreeNode* x)
{
	if (x == nullptr || x == _nilNode)
		throw runtime_error("null or nill argument in tkTreePredecessor");

	if (x->getLeftNode() != _nilNode)				//*1
		return tkTreeMaximum(x->getLeftNode());		//*2

	RedBlackTreeNode* y = x->getParentNode();		//*3
	while (y != _nilNode && x == y->getLeftNode())	//*4
	{
		x = y;										//*5
		y = y->getParentNode();						//*6
	}
	return y;										//*7
}

void RedBlackTree::tkTreeInsert(RedBlackTreeNode* z)
{
	if (z == nullptr || z == _nilNode)
		throw runtime_error("null or nill argument in tkTreeInsert");

	RedBlackTreeNode* y = _nilNode;					//*1
	RedBlackTreeNode* x = _rootNode;				//*2

	while (x != _nilNode)							//*3
	{
		y = x;										//*4
		if (StringUtils::compareIgnoringCase(z->getKey(), x->getKey()) < 0)	//*5
			x = x->getLeftNode();					//*6
		else
			x = x->getRightNode();					//*7
	}

	z->setParentNode(y);							//*8
	if (y == _nilNode)								//*9
		_rootNode = z;								//*10
	else
	{
		if (StringUtils::compareIgnoringCase(z->getKey(), y->getKey()) < 0)	//*11
			y->setLeftNode(z);						//*12
		else
			y->setRightNode(z);						//*13
	}
}

RedBlackTreeNode* RedBlackTree::tkTreeDelete(RedBlackTreeNode* z)
{
	if (z == nullptr || z == _nilNode)
		throw runtime_error("null or nil argument in tkTreeDelete");

	RedBlackTreeNode* x;
	RedBlackTreeNode* y;

	if (z->getLeftNode() == _nilNode || z->getRightNode() == _nilNode)		//*1
		y = z;																//*2
	else
		y = tkTreeSuccessor(z);												//*3

	if (y->getLeftNode() != _nilNode)										//*4
		x = y->getLeftNode();												//*5
	else
		x = y->getRightNode();												//*6

	if (x != _nilNode)														//*7
		x->setParentNode(y->getParentNode());								//*8

	if (y->getParentNode() != _nilNode)										//*9
		_rootNode = x;														//*10
	else
	{
		if (y == y->getParentNode()->getLeftNode())							//*11
			y->getParentNode()->setLeftNode(x);								//*12
		else
			y->getParentNode()->setRightNode(x);							//*13
	}

	if (y != z)																//*14
	{
		z->setKey(y->getKey());												//*15
		z->setValue(y->getValue());											//*16
	}

	return y;																//*17
}

void RedBlackTree::tkLeftRotate(RedBlackTreeNode* x)
{
	if (x == nullptr || x == _nilNode)
		throw runtime_error("null or nil argument in tkLeftRotate");
	if (x->getRightNode() == nullptr || x->getRightNode() == _nilNode)
		throw runtime_error("null or nil right child in tkLeftRotate");

	RedBlackTreeNode* y = x->getRightNode();				//*1
	x->setRightNode(y->getLeftNode());						//*2

	if (y->getLeftNode() != _nilNode)						//*3
		y->getLeftNode()->setParentNode(x);					//*4

	y->setParentNode(x->getParentNode());					//*5

	if (x->getParentNode() == _nilNode)						//*6
		_rootNode = y;										//*7
	else
	{
		if (x == x->getParentNode()->getLeftNode())			//8
			x->getParentNode()->setLeftNode(y);				//*9
		else
			x->getParentNode()->setRightNode(y);			//*10
	}

	y->setLeftNode(x);										//*11
	x->setParentNode(y);									//*12
}

void RedBlackTree::tkRightRotate(RedBlackTreeNode* x)
{
	if (x == nullptr || x == _nilNode)
		throw runtime_error("null or nil argument in tkRightRotate");
	if (x->getLeftNode() == nullptr || x->getLeftNode() == _nilNode)
		throw runtime_error("null or nil left child in tkRightRotate");

	RedBlackTreeNode* y = x->getLeftNode();					//*1
	x->setLeftNode(y->getRightNode());						//*2

	if (y->getRightNode() != _nilNode)						//*3
		y->getRightNode()->setParentNode(x);				//*4

	y->setParentNode(x->getParentNode());					//*5

	if (x->getParentNode() == _nilNode)						//*6
		_rootNode = y;										//*7
	else
	{
		if (x == x->getParentNode()->getRightNode())		//*8
			x->getParentNode()->setRightNode(y);			//*9
		else
			x->getParentNode()->setLeftNode(y);				//*10
	}

	y->setRightNode(x);										//*11
	x->setParentNode(y);									//*12
}

void RedBlackTree::tkRBInsert(RedBlackTreeNode* x)
{
	if (x == nullptr || x == _nilNode)
		throw runtime_error("null or nill argument in tkRBInsert");

	tkTreeInsert(x);																			//*1
	x->setColor(RedBlackTreeNode::Color::Red);													//*2

	while (x != _rootNode && x->getParentNode()->getColor() == RedBlackTreeNode::Color::Red)	//*3
	{
		if (x->getParentNode() == x->getParentNode()->getParentNode()->getLeftNode())			//*4
		{
			RedBlackTreeNode* y = x->getParentNode()->getParentNode()->getRightNode();			//*5
			if (y->getColor() == RedBlackTreeNode::Color::Red)									//*6
			{
				x->getParentNode()->setColor(RedBlackTreeNode::Color::Black);					//*7
				y->setColor(RedBlackTreeNode::Color::Black);									//*8
				x->getParentNode()->getParentNode()->setColor(RedBlackTreeNode::Color::Red);	//*9
				x = x->getParentNode()->getParentNode();										//*10
			}
			else
			{
				if (x == x->getParentNode()->getRightNode())									//*11
				{
					x = x->getParentNode();														//*12
					tkLeftRotate(x);															//*13
				}
				x->getParentNode()->setColor(RedBlackTreeNode::Color::Black);					//*14
				x->getParentNode()->getParentNode()->setColor(RedBlackTreeNode::Color::Red);	//*15
				tkRightRotate(x->getParentNode()->getParentNode());								//*16
			}
		}
		else
		{
			RedBlackTreeNode* y = x->getParentNode()->getParentNode()->getLeftNode();			//*5
			if (y->getColor() == RedBlackTreeNode::Color::Red)									//*6
			{
				x->getParentNode()->setColor(RedBlackTreeNode::Color::Black);					//*7
				y->setColor(RedBlackTreeNode::Color::Black);									//*8
				x->getParentNode()->getParentNode()->setColor(RedBlackTreeNode::Color::Red);	//*9
				x = x->getParentNode()->getParentNode();										//*10
			}
			else
			{
				if (x == x->getParentNode()->getLeftNode())										//*11
				{
					x = x->getParentNode();														//*12
					tkRightRotate(x);															//*13
				}
				x->getParentNode()->setColor(RedBlackTreeNode::Color::Black);					//*14
				x->getParentNode()->getParentNode()->setColor(RedBlackTreeNode::Color::Red);	//*15
				tkLeftRotate(x->getParentNode()->getParentNode());								//*16
			}
		}
	}
	_rootNode->setColor(RedBlackTreeNode::Color::Black);										//*18
}

RedBlackTreeNode* RedBlackTree::tkRBDelete(RedBlackTreeNode* z)
{
	if (z == nullptr || z == _nilNode)
		throw runtime_error("null or nill argument in tkRBDelete");

	RedBlackTreeNode* x;
	RedBlackTreeNode* y;

	if (z->getLeftNode() == _nilNode || z->getRightNode() == _nilNode)		//*1
		y = z;																//*2
	else
		y = tkTreeSuccessor(z);												//*3

	if (y->getLeftNode() != _nilNode)										//*4
		x = y->getLeftNode();												//*5
	else
		x = y->getRightNode();												//*6

	x->setParentNode(y->getParentNode());									//*7

	if (y->getParentNode() == _nilNode)										//*8
		_rootNode = x;														//*9
	else
	{
		if (y == y->getParentNode()->getLeftNode())							//*10
			y->getParentNode()->setLeftNode(x);								//*11
		else
			y->getParentNode()->setRightNode(x);							//*12
	}

	if (y != z)																//*13
	{
		z->setKey(y->getKey());												//*14
		z->setValue(y->getValue());											//*15
	}

	if (y->getColor() == RedBlackTreeNode::Color::Black)					//*16
		tkRBDeleteFixup(x);													//*17

	return y;																//*18
}

void RedBlackTree::tkRBDeleteFixup(RedBlackTreeNode* x)
{
//	if (x == nullptr || x == _nilNode)
//		throw runtime_error("null or nill argument in tkRBDeleteFixup");

	while (x != _rootNode && x->getColor() == RedBlackTreeNode::Color::Black)	//*1
	{
		if (x == x->getParentNode()->getLeftNode())								//*2
		{
			RedBlackTreeNode* w = x->getParentNode()->getRightNode();			//*3
			if (w->getColor() == RedBlackTreeNode::Color::Red)					//*4
			{
				w->setColor(RedBlackTreeNode::Color::Black);					//*5
				x->getParentNode()->setColor(RedBlackTreeNode::Color::Red);		//*6
				tkLeftRotate(x->getParentNode());								//*7
				w = x->getParentNode()->getRightNode();							//*8
			}
			if (w->getLeftNode()->getColor() == RedBlackTreeNode::Color::Black &&
				w->getRightNode()->getColor() == RedBlackTreeNode::Color::Black)	//*9
			{
				w->setColor(RedBlackTreeNode::Color::Red);						//*10
				x = x->getParentNode();											//*11
			}
			else
			{
				if (w->getRightNode()->getColor() == RedBlackTreeNode::Color::Black)//*12
				{
					w->getLeftNode()->setColor(RedBlackTreeNode::Color::Black);	//*13
					w->setColor(RedBlackTreeNode::Color::Red);					//*14
					tkRightRotate(w);											//*15
					w = x->getParentNode()->getRightNode();							//*16
				}
				w->setColor(x->getParentNode()->getColor());					//*17
				x->getParentNode()->setColor(RedBlackTreeNode::Color::Black);	//*18
				w->getRightNode()->setColor(RedBlackTreeNode::Color::Black);	//*19
				tkLeftRotate(x->getParentNode());								//*20
				x = _rootNode;													//*21
			}
		}
		else																	//*22
		{
			RedBlackTreeNode* w = x->getParentNode()->getLeftNode();			//*3
			if (w->getColor() == RedBlackTreeNode::Color::Red)					//*4
			{
				w->setColor(RedBlackTreeNode::Color::Black);					//*5
				x->getParentNode()->setColor(RedBlackTreeNode::Color::Red);		//*6
				tkRightRotate(x->getParentNode());								//*7
				w = x->getParentNode()->getLeftNode();							//*8
			}
			if (w->getRightNode()->getColor() == RedBlackTreeNode::Color::Black &&
				w->getLeftNode()->getColor() == RedBlackTreeNode::Color::Black)	//*9
			{
				w->setColor(RedBlackTreeNode::Color::Red);						//*10
				x = x->getParentNode();											//*11
			}
			else
			{
				if (w->getLeftNode()->getColor() == RedBlackTreeNode::Color::Black)	//*12
				{
					w->getRightNode()->setColor(RedBlackTreeNode::Color::Black);//*13
					w->setColor(RedBlackTreeNode::Color::Red);					//*14
					tkLeftRotate(w);											//*15
					w = x->getParentNode()->getLeftNode();							//*16
				}
				w->setColor(x->getParentNode()->getColor());					//*17
				x->getParentNode()->setColor(RedBlackTreeNode::Color::Black);	//*18
				w->getLeftNode()->setColor(RedBlackTreeNode::Color::Black);		//*19
				tkRightRotate(x->getParentNode());								//*20
				x = _rootNode;													//*21
			}
		}
	}
	x->setColor(RedBlackTreeNode::Color::Black);								//*23
}

void RedBlackTree::clearTree()
{
	clearSubTree(_rootNode);
	_rootNode = _nilNode;
}

void RedBlackTree::clearSubTree(RedBlackTreeNode* node)
{
	if (node == nullptr || node == _nilNode)
		return;
	clearSubTree(node->getLeftNode());
	clearSubTree(node->getRightNode());
	delete node;
}

void RedBlackTree::saveSubTree(RedBlackTreeNode* node, ostream& stream)
{
	if (node == nullptr || node == _nilNode)
		return;

	saveSubTree(node->getLeftNode(), stream);

	string key = node->getKey();
	size_t length = key.length();
	uint64_t value = node->getValue();

	stream << key << " " << value << endl;		// no spaces in key

	saveSubTree(node->getRightNode(), stream);
}
