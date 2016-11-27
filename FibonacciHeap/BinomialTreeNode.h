#ifndef binomial_tree_node_
#define binomial_tree_node_

#include "DoubleLinkedList.h"
#include <string>

template<typename TKey, typename TValue>
class BinomialTreeNode
{
	typedef BinomialTreeNode<TKey, TValue> self_t;

	//prvek (key a value)
	TKey key;
	TValue value;

	DoubleLinkedList<self_t*>* sons;

	bool lostSon = false;

	
public:

	



	bool get_lost_son() const
	{
		return lostSon;
	}

	void reset_lost_son()
	{
		lostSon = false;
	}
	
	void set_lost_son()
	{
		if (lostSon)
			throw "Already lost son!";

		lostSon = true;
	}

	const TKey& get_key() const
	{
		return key;
	}

	void set_key(const TKey& new_key)
	{
		key = new_key;
	}

	const TValue& get_value() const
	{
		return value;
	}

	//otec
	ListNode<self_t*>* parent;

	//levy a pravy bratr
	//self_t* leftBrother;
	//self_t* rightBrother;

	//rad podstromu
	TKey sub_tree_order;


	DoubleLinkedList<self_t*>& get_sons() const
	{
		return *sons;
	}

	DoubleLinkedList<self_t*>* take_sons_ownership()
	{
		auto* it = sons->get_first();
		while (it != nullptr)
		{
			it->value->parent = nullptr;
			it = it->next;
		}

		DoubleLinkedList<self_t*>* res = sons;
		sons = nullptr;
		return res;
	}

	BinomialTreeNode(const TKey& key_p, const TValue& val_p) :
		key(key_p),
		value(val_p),
		parent(nullptr),
		sons(new DoubleLinkedList<self_t*>()),
		sub_tree_order(0)
	{};

	virtual ~BinomialTreeNode()
	{
		if (sons != nullptr)
			delete sons;
	}
};

#endif
