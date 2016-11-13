#ifndef naive_splay_tree
#define naive_splay_tree

#include "SplayTree.h"

template<typename TKey, typename Tval>
class NaiveSplayTree : public SplayTree<TKey, Tval>
{
protected:
	virtual void ZigZig(typename SplayTree<TKey, Tval>::Node_ptr x) override
	{
		SplayTree<TKey, Tval>::ZigZag(x);
	}

public:
	virtual ~NaiveSplayTree() {};
};


#endif
