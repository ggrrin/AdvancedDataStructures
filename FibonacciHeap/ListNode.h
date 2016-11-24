#ifndef list_node_
#define list_node_

#include <cstdint>

template<typename TValue>
class ListNode
{
public:
	ListNode* next;
	ListNode* previous;
	TValue value;

	explicit ListNode(TValue val) :
		next(nullptr),
		previous(nullptr),
		value(val) {};

	virtual ~ListNode()
	{
		if(value != nullptr)
			delete value;
	}

	ListNode<TValue>* operator++() const
	{
		return next;
	}

	TValue operator*() const
	{
		return value;
	}

	TValue operator->() const
	{
		return  value;
	}

};

#endif