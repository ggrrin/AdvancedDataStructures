#ifndef naive_fibonacci_heap_
#define naive_fibonacci_heap_

#include "FibonacciHeap.h"

template<typename TKey, typename TValue>
class NaiveFibonacciHeap : public FibonacciHeap<TKey,TValue>
{
public:
	virtual void decrease_key(FibonacciHeap<TKey,TValue>::node* u, const TKey& newKey) override
	{
		u->value->set_key(newKey);

		auto* p = u->value->parent;
		if (p == nullptr || p->value->get_key() <= u->value->get_key())
			return;

		u->value->reset_lost_son();
		u->value->parent->value->get_sons().remove(u);
		u->value->parent = nullptr;
		binomialTrees.add_node(u);


		if (newKey < treeWithMin->value->get_key())
			treeWithMin = u;

		while (p->value->parent != nullptr && p->value->get_lost_son())
		{
			u = p;
			p = u->value->parent;
			u->value->reset_lost_son();
			u->value->parent->value->get_sons().remove(u);
			u->value->parent = nullptr;
			binomialTrees.add_node(u);
		}

		if (p->value->parent != nullptr)
			p->value->set_lost_son();

		dump();
	};


};


#endif
