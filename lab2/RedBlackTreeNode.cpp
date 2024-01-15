#include "RedBlackTreeNode.h"

RedBlackTreeNode::RedBlackTreeNode(const string& key, const uint64_t value, Color color,
	RedBlackTreeNode* parentNode, RedBlackTreeNode* leftNode, RedBlackTreeNode* rightNode)
{
	_key = key;
	_value = value;
	_color = color;
	_parentNode = parentNode;
	_leftNode = leftNode;
	_rightNode = rightNode;
}

RedBlackTreeNode::~RedBlackTreeNode()
{
}

string RedBlackTreeNode::getKey()
{
	return _key;
}

void RedBlackTreeNode::setKey(const string& key)
{
	_key = key;
}

uint64_t RedBlackTreeNode::getValue()
{
	return _value;
}

void RedBlackTreeNode::setValue(const uint64_t value)
{
	_value = value;
}

RedBlackTreeNode::Color RedBlackTreeNode::getColor()
{
	return _color;
}

void RedBlackTreeNode::setColor(Color color)
{
	_color = color;
}

RedBlackTreeNode* RedBlackTreeNode::getParentNode()
{
	return _parentNode;
}

void RedBlackTreeNode::setParentNode(RedBlackTreeNode* node)
{
	_parentNode = node;
}

RedBlackTreeNode* RedBlackTreeNode::getLeftNode()
{
	return _leftNode;
}

void RedBlackTreeNode::setLeftNode(RedBlackTreeNode* node)
{
	_leftNode = node;
}

RedBlackTreeNode* RedBlackTreeNode::getRightNode()
{
	return _rightNode;
}

void RedBlackTreeNode::setRightNode(RedBlackTreeNode* node)
{
	_rightNode = node;
}
