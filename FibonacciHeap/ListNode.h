#ifndef list_node_
#define list_node_

#include <cstdint>
#include <string>
#include <stdio.h>

void thr(char* message)
{
	printf("%s\n", message);
	throw message;
}

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
		if (value != nullptr)
			delete value;
	}

	void null_list()
	{
		next = nullptr;
		previous = nullptr;
	}


	std::string dump_s;

	std::string dump()
	{

		dump_s = "[";
		dump_s += std::to_string(value->get_key());
		dump_s += "]";

		if (!value->get_sons().empty())
		{
			dump_s += "(";

			auto* it = value->get_sons().begin();
			while (it != nullptr)
			{
				dump_s += it->dump();
				it = it->next;
			}

			dump_s += ")";
		}

		return dump_s;
	}
};

#endif