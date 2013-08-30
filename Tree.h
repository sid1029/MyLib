#ifndef TREE_H
#define TREE_H

#include "CoreTypes.h"
#include <vector>
#include <iostream>
#include <sstream>

namespace graph {

class Tree
{
public:
	struct Node
	{
		U32 data;
		Node* leftChild;
		Node* rightChild;
		Node(U32 elem) : data(elem), leftChild(0), rightChild(0) {};
		~Node();
	};

	Node* root;
	Tree() : root(0) { }
	virtual bool PushElement(Node* &node, U32 input) = 0;
	virtual void PopElement(Node* &node, U32 key) = 0;
	virtual Node* SearchElement(Node* node, U32 key) = 0;
};

class BSTree : public Tree
{
public:
	BSTree() : numCompares(0) { }
	BSTree(const BSTree& other);
	Node* CopyTree(const Node* node);
	virtual bool PushElement(Node* &node, U32 input);
	virtual void PopElement(Node* &node, U32 key);
	virtual Node* SearchElement(Node* node, U32 key);

	void PrintInOrder(Node* node);
	void PrintPostOrder(Node* node);
	void PrintPreOrder(Node* node);
	void LowestCommonAncestor(Node* node, U32 n1, U32 n2);
	U32 MaxSumToLeaf(Node* node);
	U32 FindKthSmallest(Node* node, int k);
	void TraverseBFS(Node* node);
	void TraverseDFS(Node* node);
private:
	bool HasAncestors(Node* node, U32 n1, U32 n2);
	U32 numCompares;
};

class BinaryTree : public Tree
{
public:
	BinaryTree() : Tree() {}
	BinaryTree(const BinaryTree& other);
	Node* CopyTree(const Node* node);
	virtual bool PushElement(Node* &node, U32 input);
	virtual void PopElement(Node* &node, U32 key);
	virtual Node* SearchElement(Node* node, U32 key);

	void PrintInOrder(Node* node);
	void PrintPostOrder(Node* node);
	void PrintPreOrder(Node* node);
	void TraverseBFS(Node* node);
	void TraverseDFS(Node* node);
	bool IsSymmetric(Node* node);
private:
	void BuildInOrderStr(Node* node, std::ostringstream* s);
	void BuildPostOrderStr(Node* node, std::ostringstream* s);
};

class Graph
{
public:
	struct GraphNode
	{
		U32 data;
		std::vector<GraphNode*> children;
		GraphNode(U32 elem) : data(elem), children(2) {};
		~GraphNode();
	};

	GraphNode* root;
	Graph() : root(0) {};
	virtual bool PushElement(GraphNode* &node, U32 key) = 0;
	virtual void PopElement(GraphNode* &node, U32 key) = 0;
	virtual GraphNode* SearchElement(GraphNode* node, U32 key) = 0;
};

class DiGraph : public Graph
{
public:
	DiGraph() : numCompares(0) {};
	virtual bool PushElement(GraphNode* &node, U32 key);
	virtual void PopElement(GraphNode* &node, U32 key);
	virtual GraphNode* SearchElement(GraphNode* node, U32 key);

	void PrintInOrder(GraphNode* node);
	void PrintPostOrder(GraphNode* node);
	void PrintPreOrder(GraphNode* node);

	U32 numCompares;
};

}
#endif // TREE_H