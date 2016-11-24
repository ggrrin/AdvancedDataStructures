#ifndef fibonacci_heap_
#define fibonacci_heap_

#include <math.h>
#include "BinomialTreeNode.h"


template<typename TKey, typename TValue>
class FibonacciHeap
{
	//kazdy prvek je v prave jednom vrcholu jednoho binarniho stromu
	//kazdy binomialni strom je halda
	//zadne dva binomialni stromy nemaji stejny rad
	//binomialni stromy jsou ulozeny v seznamu pomoci odkazu na bratry
	//odstraneni korene vznikne binom hlada v O 1

	typedef BinomialTreeNode<TKey, TValue> node_t;
	typedef node_t sub_tree_root;

	//mnozina binomialnich stromu
	DoubleLinkedList<sub_tree_root*> binomialTrees;

	//udrzuje ukazatel na strom s minimem
	ListNode<sub_tree_root*>* treeWithMin;

	int size;

	sub_tree_root* join_binomial_sub_heaps(sub_tree_root* first, sub_tree_root* second)
	{
		if (first->sub_tree_order != second->sub_tree_order)
			throw "Orders does not fit.";

		if (first->get_key() > second->get_key())
		{
			auto* temp = second;
			second = first;
			first = temp;
		}

		first->get_sons().add(second);
		second->parent = first;
		first->parent = nullptr;
		++first->sub_tree_order;

		return first;
	}


	void reconstruct_heap()
	{
		if (binomialTrees.empty())
		{
			treeWithMin = nullptr;
			return;
		}

		size_t orders = 1 + static_cast<size_t>(ceil(log2(get_size())));
		sub_tree_root** roots = new sub_tree_root*[orders];

		for (size_t i = 0; i < orders; i++)
			roots[i] = nullptr;

		for (auto current = binomialTrees.begin();
			current != binomialTrees.end();
			current = ++*current)
		{
			sub_tree_root* h = **current;
			int cur_order = h->sub_tree_order;

			while (roots[cur_order] != nullptr)
			{
				h = join_binomial_sub_heaps(h, roots[cur_order]);
				roots[cur_order] = nullptr;
				cur_order++;
			}

			roots[cur_order] = h;
		}

		binomialTrees.clear_all(true);

		for (size_t i = 0; i < orders; i++)
		{
			if(roots[i] != nullptr)
			{
				binomialTrees.add(roots[i]);

				if (treeWithMin == nullptr || roots[i]->get_key() < (*treeWithMin)->get_key())
				{
					treeWithMin = binomialTrees.get_last();
				}
			}
		}

		delete[] roots;
	}



public:
	FibonacciHeap() :
		binomialTrees(),
		treeWithMin(nullptr),
		size(0)
	{}

	int get_size() const
	{
		return size;
	};

	virtual const node_t& find_min() const
	{
		return *treeWithMin->value;
	};


	virtual void insert(const TKey& key, const TValue& val)
	{
		node_t* new_tree = new node_t(key, val);
		bool reset_min = binomialTrees.empty() || (*treeWithMin)->get_key() > new_tree->get_key();

		binomialTrees.add(new_tree);

		if(reset_min)
			treeWithMin = binomialTrees.get_last();

		size++;
	};

	virtual void delete_min()
	{
		if (treeWithMin != nullptr)
		{
			auto* sons = (*treeWithMin)->take_sons_ownership();
			//delete treeWithMin;
			binomialTrees.remove(treeWithMin);
			treeWithMin = nullptr;

			size--;

			binomialTrees.MergeWithAndDestroy(sons);
			reconstruct_heap();
		}
	};

	virtual void decrease_key(const TKey& key, const TKey& newKey)
	{
		
	};
};


#endif 