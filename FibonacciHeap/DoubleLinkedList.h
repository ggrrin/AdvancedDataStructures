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
		while (it != nullptr)
		{
			auto* to_del = it;
			it = it->previous;
			remove(to_del);
			delete to_del;
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

	ListNode<TValue>* add(const TValue val)
	{
		auto* new_node = new ListNode<TValue>(val);
		add_node(new_node);
		return new_node;
	}

	void add_node(ListNode<TValue>* new_node)
	{
		new_node->null_list();

		if (last == nullptr || first == nullptr)
		{
			if (last != first)
				thr("Structure is broken");

			first = last = new_node;
		}
		else
		{
			last->next = new_node;
			last->next->previous = last;
			last = last->next;
		}
	}

	void reset_no_delete()
	{
		first = last = nullptr;
	}

	void merge_with_and_destroy(DoubleLinkedList* list)
	{
		if (!list->empty())
		{
			if (last == nullptr || first == nullptr)
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

		list->reset_no_delete();
		delete list;
	}

	ListNode<TValue>* remove(ListNode<TValue>* node)
	{
		if (node == first && node == last)
		{
			first = last = nullptr;
		}
		else if (node->previous == nullptr)
		{
			first = node->next;
			first->previous = nullptr;
		}
		else if (node->next == nullptr)
		{
			last = node->previous;
			last->next = nullptr;
		}
		else
		{
			node->previous->next = node->next;
			node->next->previous = node->previous;
		}

		node->null_list();
		return node;
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
