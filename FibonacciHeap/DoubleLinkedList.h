#ifndef double_linked_list_
#define double_linked_list_

#include "ListNode.h"

template<typename TValue>
class DoubleLinkedList
{
	ListNode<TValue>* first;
	ListNode<TValue>* last;

public:
	DoubleLinkedList() :
		first(nullptr),
		last(nullptr)
	{ }

	virtual ~DoubleLinkedList()
	{
		auto* it = last;
		while(it != nullptr)
		{
			delete it;
			it = it->previous;
		}
	}

	ListNode<TValue>* get_first()
	{
		return first;
	};
	
	ListNode<TValue>* get_last()
	{
		return last;
	};

	void add(const TValue val)
	{
		if (last == nullptr || first == nullptr)
		{
			if (last != first)
				throw "Structure is broken";

			first = last = new ListNode<TValue>(val);
		}
		else
		{
			last->next = new ListNode<TValue>(val);
			last->next->previous = last;
			last = last->next;
		}
	}

	void clear_all(bool node_only = false)
	{
		while(!empty())
			remove(get_first(), node_only);
	}

	void MergeWithAndDestroy(DoubleLinkedList* list)
	{
		if (!list->empty())
		{
			if (last == nullptr)
			{
				first = list->first;
				last = list->last;
			}
			else
			{
				last->next = list->first;
				list->first->previous = last;
				last = list->last;
			}
		}

		first = last = nullptr;
		delete list;
	}

	void remove(ListNode<TValue>* node, bool node_only = false)
	{

		if(node == first && node == last)
		{
			first = last = nullptr;
		}
		else if (node->previous == nullptr)
		{
			first = node->next;
			first->previous = nullptr;
		}
		else if(node->next == nullptr)
		{
			last = node->previous;
			last->next = nullptr;
		}
		else
		{
			node->previous->next = node->next;
			node->next->previous = node->previous;
		}

		if(node_only)
		{
			node->value = nullptr;
		}

		delete node;
	}

	bool empty() const
	{
		return first == nullptr;
	}

	ListNode<TValue>* begin() const
	{
		return  first;
	}

	ListNode<TValue>* end() const
	{
		return nullptr;
	}
};

#endif
