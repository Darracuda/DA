#pragma once

#include <string>
#include <iostream> 
#include <fstream>
#include "RedBlackTreeNode.h"

using namespace std;

class RedBlackTree
{
private:
	RedBlackTreeNode* _rootNode;
	RedBlackTreeNode* _nilNode;

public:
	RedBlackTree();
	virtual ~RedBlackTree();

	bool add(const string& key, const uint64_t value);
	bool remove(const string& key);
	bool get(const string& key, uint64_t* pValue);
	void saveToFile(const string& filename);
	static RedBlackTree* createFromFile(const string& filename);

private:
	RedBlackTreeNode* findNode(const string& key);

	// Tomas Kormen - Binary Tree
	RedBlackTreeNode* tkTreeMinimum(RedBlackTreeNode* x);
	RedBlackTreeNode* tkTreeMaximum(RedBlackTreeNode* x);
	RedBlackTreeNode* tkTreeSuccessor(RedBlackTreeNode* x);
	RedBlackTreeNode* tkTreePredecessor(RedBlackTreeNode* x);
	void tkTreeInsert(RedBlackTreeNode* z);
	RedBlackTreeNode* tkTreeDelete(RedBlackTreeNode* z);

	// Tomas Kormen - Red-Black Tree
	void tkLeftRotate(RedBlackTreeNode* x);
	void tkRightRotate(RedBlackTreeNode* x);
	void tkRBInsert(RedBlackTreeNode* x);
	RedBlackTreeNode* tkRBDelete(RedBlackTreeNode* z);
	void tkRBDeleteFixup(RedBlackTreeNode* x);

	void clearTree();
	void clearSubTree(RedBlackTreeNode* node);
	void saveSubTree(RedBlackTreeNode* node, ostream& stream);
};

