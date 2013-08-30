#include "Tree.h"
#include <iostream>
#include <assert.h>
#include <queue>

namespace graph {

using namespace std;

Tree::Node::~Node()
{
	delete leftChild;
	delete rightChild;
}

BSTree::BSTree( const BSTree& other )
{
	this->numCompares = 0;
	if (other.root)
		this->root = CopyTree(other.root);
}

BSTree::Node* BSTree::CopyTree( const BSTree::Node* node )
{
	BSTree::Node* newNode  = new BSTree::Node(node->data);
	if (node->leftChild)
		newNode->leftChild = CopyTree(node->leftChild);
	if (node->rightChild)
		newNode->rightChild = CopyTree(node->rightChild);
	return newNode;
}

bool BSTree::PushElement(Tree::Node* &node, U32 input)
{
	if (node == NULL)
	{
		node = new Node(input);
		return true;
	}
	if (input < node->data)
		PushElement(node->leftChild, input);
	else
		PushElement(node->rightChild, input);
	return true;
}

void BSTree::PopElement(Tree::Node* &node, U32 key)
{
	if (node->data == key)
	{
		delete node;
		return;
	}
	if (key < node->data)
		PopElement(node->leftChild, key);
	else
		PopElement(node->rightChild, key);
}

Tree::Node* BSTree::SearchElement(Tree::Node* node, U32 key)
{
	++numCompares;
	std::cout<<"Comparison "<<numCompares<<" : Checked "<<node->data<<"\n";
	if (node->data == key) return node;
	if (key < node->data && node->leftChild) return SearchElement(node->leftChild, key);
	else if (node->rightChild) return SearchElement(node->rightChild, key);
	return NULL;
}

void BSTree::PrintInOrder(Node* node)
{
	if (!node)
		return;
	PrintInOrder(node->leftChild);
	std::cout<<node->data<<" ";
	PrintInOrder(node->rightChild);
}

void BSTree::PrintPreOrder(Node* node)
{
	if (!node)
		return;
	std::cout<<node->data<<" ";
	PrintPreOrder(node->leftChild);
	PrintPreOrder(node->rightChild);
}

void BSTree::PrintPostOrder(Node* node)
{
	if (!node)
		return;
	PrintPostOrder(node->rightChild);
	PrintPostOrder(node->leftChild);
	std::cout<<node->data<<" ";
}

bool BSTree::HasAncestors(Node* node, U32 n1, U32 n2)
{
	if (!node)
		return false;
	if (node->data == n1 || node->data == n2)
		return true;
	else
	{
		bool leftSubTree = false, rightSubTree = false;
		leftSubTree = HasAncestors(node->leftChild, n1, n2);
		rightSubTree = HasAncestors(node->rightChild, n1, n2);
		if (leftSubTree && rightSubTree)
		{
			std::cout<<"Found LCA at "<<node->data;
			return true;
		}
		return leftSubTree || rightSubTree;
	}
}

void BSTree::LowestCommonAncestor(Node* node, U32 n1, U32 n2)
{
	if (!node)
		return;

	if (n1 > node->data && n2 > node->data)
		LowestCommonAncestor(node->leftChild, n1, n2);

	if (n1 < node->data && n2 < node->data)
		LowestCommonAncestor(node->rightChild, n1, n2);

	if ((n1 < node->data && n2 > node->data) || (n1 > node->data && n2 < node->data))
		std::cout<<"Found LCA at "<<node->data;
}

U32 BSTree::FindKthSmallest(Node* node, int k)
{
	if (!node)
		return 0;
	k = FindKthSmallest(node->leftChild, k);
	if (k < 4)
		k++;
	if (k == 4)
	{
		cout<<node->data;
		return 0;
	}
	k = FindKthSmallest(node->rightChild, k);
}

void BSTree::TraverseBFS(Node* node)
{
	std::queue<Node*> bfsQueue;
	bfsQueue.push(node);
	while (!bfsQueue.empty())
	{
		Node* curr = bfsQueue.front();
		bfsQueue.pop();
		if (curr->leftChild)
			bfsQueue.push(curr->leftChild);
		if (curr->rightChild)
			bfsQueue.push(curr->rightChild);
		std::cout<<curr->data<<" ";
	}
}

void BSTree::TraverseDFS(Node* node)
{
	std::cout<<node->data<<" ";
	if (node->leftChild)
		TraverseDFS(node->leftChild);
	if (node->rightChild)
		TraverseDFS(node->rightChild);
}

U32 BSTree::MaxSumToLeaf( Node* node )
{
	U32 leftSum = 0, rightSum = 0;
	if (node->leftChild)
		leftSum = node->data + MaxSumToLeaf(node->leftChild);
	if (node->rightChild)
		rightSum = node->data + MaxSumToLeaf(node->rightChild);
	return std::max(leftSum, rightSum);
}

bool BinaryTree::IsSymmetric( Node* node )
{
	if (node->leftChild && node->rightChild)
	{
		ostringstream lstr;
		ostringstream rstr;
		BuildInOrderStr(node->leftChild, &lstr);
		BuildPostOrderStr(node->rightChild, &rstr);

		cout<<lstr.str()<<endl;
		cout<<rstr.str()<<endl;

		if (lstr.str() == rstr.str())
			return true;
	}
	return false;
}

void BinaryTree::BuildInOrderStr( Node* node, std::ostringstream* s )
{
	if (node->leftChild) BuildInOrderStr(node->leftChild, s);
	s->operator<<(node->data);
	if (node->rightChild) BuildInOrderStr(node->rightChild, s);
}

void BinaryTree::BuildPostOrderStr( Node* node, std::ostringstream* s )
{
	if (node->rightChild) BuildPostOrderStr(node->rightChild, s);
	if (node->leftChild) BuildPostOrderStr(node->leftChild, s);
	s->operator<<(node->data);
}

bool BinaryTree::PushElement( Node* &node, U32 input )
{
	if (!node)
	{
		node = new Node(input);
		return true;
	}
	else if (!node->leftChild)
	{
		node->leftChild = new Node(input);
		return true;
	}
	else if (!node->rightChild)
	{
		node->rightChild = new Node(input);
		return true;
	}
	else
	{
		return PushElement(node->leftChild, input);
	}
}

void BinaryTree::PopElement( Node* &node, U32 key )
{
	if (node->data == key)
	{
		delete node;
		return;
	}
	if (key < node->data)
		PopElement(node->leftChild, key);
	else
		PopElement(node->rightChild, key);
}

Tree::Node* BinaryTree::SearchElement( Tree::Node* node, U32 key )
{
	std::queue<Tree::Node*> bfsQueue;
	bfsQueue.push(node);
	if (node->data == key)
		return node;
	while (!bfsQueue.empty())
	{
		Node* curr = bfsQueue.front();
		bfsQueue.pop();
		if (curr->leftChild)
			bfsQueue.push(curr->leftChild);
		if (curr->rightChild)
			bfsQueue.push(curr->rightChild);
	}
}
} // namespace graph