#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "CoreTypes.h"

namespace list
{
	class Node
	{
	public:
		U32 data;
		Node* next;
		Node() : data(0), next(0) {}
		Node(U32 elem) : data(elem), next(0) {}
	};

	template <typename NodeType>
	class LinkedList
	{
	public:
		LinkedList()
		{
			mSize = 0;
			mHead = new NodeType((U32)-1);
			NodeType* prevNode = NULL;
		}

		inline NodeType* head() { return mHead->next; }
		inline U32 size() { return mSize; }

		void push(const U32 elem)
		{
			if (!mHead->next)
				mHead->next = new NodeType(elem);
			else
			{
				NodeType* node = mHead->next;
				while (node->next)
					node = node->next;
				node->next = new NodeType(elem);
			}
			++mSize;
		}

		void remove(const U32 elem)
		{
			if (!mHead->next)
				return;
			else
			{
				NodeType* node = mHead->next;
				while (node->next && node->next->data != elem)
					node = node->next;
				NodeType* toDel = node->next;
				if (node->next->next)
					node->next = node->next->next;
				else
					node->next = NULL;
				delete toDel;
				--mSize;
			}
		}

		void printList()
		{
			if (!mHead->next)
				return;
			NodeType* node = mHead->next;
			while (node)
			{
				std::cout<<node->data<<" ";
				node = node->next;
			}
		}

		void reverseList(U32 n = 0)
		{
			if (!mHead->next)
				return;
			if (n == 0)
				reverse(mHead->next);
			else
				reverse(mHead->next, n);
		}
	private:
		void reverse(NodeType* node)
		{
			if (!node)
				return;
			NodeType* nextNode = node->next;
			if (!nextNode)
			{
				mHead->next = node;
				return;
			}
			reverse(node->next);
			nextNode->next = node;
			node->next = NULL;
		}

		void reverse(NodeType* node, U32 n)
		{
			if (!node)
				return;
			NodeType* nextNode = node->next;
			if (!nextNode || n == 1)
			{
				mHead->next = node;
				node->next = nextNode;
				return;
			}
			reverse(node->next, --n);
			nextNode->next = node;
		}

	private:
		NodeType* mHead;
		U32 mSize;
	};
}
#endif // LINKEDLIST_H