#ifndef fibonacci_heap_
#define fibonacci_heap_

#include <math.h>
#include "BinomialTreeNode.h"


template<typename TKey, typename TValue>
class FibonacciHeap
{
protected:
	
	//kazdy prvek je v prave jednom vrcholu jednoho binarniho stromu
	//kazdy binomialni strom je halda
	//zadne dva binomialni stromy nemaji stejny rad
	//binomialni stromy jsou ulozeny v seznamu pomoci odkazu na bratry
	//odstraneni korene vznikne binom hlada v O 1

	typedef BinomialTreeNode<TKey, TValue> node_t;
	typedef ListNode<node_t*> sub_tree_root;

	//mnozina binomialnich stromu
	DoubleLinkedList<node_t*> binomialTrees;

	//udrzuje ukazatel na strom s minimem
	sub_tree_root* treeWithMin;

	std::int32_t size;

	sub_tree_root* join_binomial_sub_heaps(sub_tree_root* first_node, sub_tree_root* second_node)
	{
		if (first_node->value->get_key() > second_node->value->get_key())
		{
			auto* temp = second_node;
			second_node = first_node;
			first_node = temp;
		}

		auto* first = first_node->value;
		auto* second = second_node->value;

		if (first->sub_tree_order != second->sub_tree_order)
			thr("Orders does not fit.");

		first->get_sons().add_node(second_node);
		second->parent = first_node;
		first->parent = nullptr;
		++first->sub_tree_order;

		return first_node;
	}


	uint32_t get_order_estimate()
	{
		double_t size_estimate = 0;
		for (auto* it = binomialTrees.begin(); it != nullptr; it = it->next)
		{
			auto prev = size_estimate;
			size_estimate += exp2(it->value->sub_tree_order);

			if(size_estimate <  prev)
				thr("probably overflow");
		}


		return 1 + static_cast<uint32_t>(log2(size_estimate));
	}

	void reconstruct_heap()
	{
		if (binomialTrees.empty())
		{
			treeWithMin = nullptr;
			return;
		}

		auto orders = get_order_estimate();//1 + static_cast<std::int32_t>(ceil(log2(get_size())));
		sub_tree_root** roots = new sub_tree_root*[orders];

		for (std::int32_t i = 0; i < orders; i++)
			roots[i] = nullptr;

		for (;;)
		{
			if (binomialTrees.empty())
				break;

			sub_tree_root* h = binomialTrees.remove(binomialTrees.get_first());

			int32_t cur_order = h->value->sub_tree_order;
			while (roots[cur_order] != nullptr)
			{
				if (cur_order >= orders)
					thr("Consolidation array overflow.");

				steps++;
				h = join_binomial_sub_heaps(h, roots[cur_order]);
				roots[cur_order] = nullptr;
				cur_order++;
			}

			roots[cur_order] = h;
		}

		binomialTrees.reset_no_delete();

		for (std::int32_t i = 0; i < orders; i++)
		{
			if (roots[i] != nullptr)
			{
				binomialTrees.add_node(roots[i]);

				if (treeWithMin == nullptr || roots[i]->value->get_key() < treeWithMin->value->get_key())
				{
					treeWithMin = binomialTrees.get_last();
				}
			}
		}

		delete[] roots;
	}

	std::int32_t deletes;
	std::int32_t steps;


	void dump()
	{
		
	}

public:
	void dump_tree()
	{
		auto* it = binomialTrees.begin();
		while (it != nullptr)
		{
			it->dump();
			it = it->next;
		}
	}
	
	typedef ListNode<node_t*> node;

	virtual ~FibonacciHeap()
	{
	}

	FibonacciHeap() :
		binomialTrees(),
		treeWithMin(nullptr),
		size(0),
		steps(0),
		deletes(0)
	{}

	double get_average_delete_step_count() const
	{
		return static_cast<double>(steps) / static_cast<double>(deletes);
	}

	std::int32_t get_size() const
	{
		return size;
	};

	virtual const node_t& find_min() const
	{
		return *treeWithMin->value;
	};



	virtual node* insert(const TKey& key, const TValue& val)
	{
		node_t* new_tree = new node_t(key, val);
		bool reset_min = binomialTrees.empty() || treeWithMin->value->get_key() >= new_tree->get_key();

		node* res = binomialTrees.add(new_tree);

		if (reset_min)
			treeWithMin = binomialTrees.get_last();

		size++;

		dump();
		return res;
	};

	virtual node* delete_min()
	{
		if (treeWithMin != nullptr)
		{
			deletes++;

			auto* sons = treeWithMin->value->take_sons_ownership();
			//delete treeWithMin;
			auto* res = binomialTrees.remove(treeWithMin);
			treeWithMin = nullptr;

			size--;

			steps++;

			binomialTrees.merge_with_and_destroy(sons);
			reconstruct_heap();

			dump();

			return res;
		}

		return nullptr;
	};

	virtual void decrease_key(node* u, const TKey& newKey)
	{
		u->value->set_key(newKey);

		auto* p = u->value->parent;


		if (newKey < treeWithMin->value->get_key())
			treeWithMin = u;

		if (p == nullptr || p->value->get_key() <= u->value->get_key())
			return;

		u->value->reset_lost_son();
		u->value->parent->value->get_sons().remove(u);
		u->value->parent = nullptr;
		binomialTrees.add_node(u);



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

template<typename TKey, typename TValue>
class NaiveFibonacciHeap : public virtual FibonacciHeap<TKey,TValue>
{
public:
	virtual void decrease_key(typename FibonacciHeap<TKey,TValue>::node* u, const TKey& newKey) override
	{
		u->value->set_key(newKey);

		auto* p = u->value->parent;

		if (newKey < this->treeWithMin->value->get_key())
			this->treeWithMin = u;

		if (p == nullptr || p->value->get_key() <= u->value->get_key())
			return;

		u->value->reset_lost_son();
		u->value->parent->value->get_sons().remove(u);
		u->value->parent = nullptr;
		this->binomialTrees.add_node(u);



		while (p->value->parent != nullptr && p->value->get_lost_son())
		{
			u = p;
			p = u->value->parent;
			u->value->reset_lost_son();
			u->value->parent->value->get_sons().remove(u);
			u->value->parent = nullptr;
			this->binomialTrees.add_node(u);
		}

		if (p->value->parent != nullptr)
			p->value->set_lost_son();

		this->dump();
	};


};


#endif 
