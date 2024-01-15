#pragma once

#include <string>

using namespace std;

class RedBlackTreeNode
{
public:
	enum class Color { Black, Red };

private:
	string _key;
	uint64_t _value;
	Color _color;

	RedBlackTreeNode* _parentNode;
	RedBlackTreeNode* _leftNode;
	RedBlackTreeNode* _rightNode;

public:
	RedBlackTreeNode(const string& key, const uint64_t value, Color color,
		RedBlackTreeNode* parentNode, RedBlackTreeNode* leftNode, RedBlackTreeNode* rightNode);
	virtual ~RedBlackTreeNode();

	string getKey();
	void setKey(const string& key);
	uint64_t getValue();
	void setValue(const uint64_t value);
	Color getColor();
	void setColor(Color color);

	RedBlackTreeNode* getParentNode();
	void setParentNode(RedBlackTreeNode* node);
	RedBlackTreeNode* getLeftNode();
	void setLeftNode(RedBlackTreeNode* node);
	RedBlackTreeNode* getRightNode();
	void setRightNode(RedBlackTreeNode* node);
};

