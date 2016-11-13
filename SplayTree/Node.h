#ifndef tree_node_
#define tree_node_


template<typename TKey, typename TVal>
class Node
{
public:
	typedef Node<TKey, TVal>* Node_ptr;
	typedef Node<TKey, TVal> NodeT;

	explicit Node(const TKey& key_p) :
		key(key_p),
		value(TVal()),
		parent(nullptr),
		left(nullptr),
		right(nullptr)
	{ };

	~Node()
	{
		if(left != nullptr)
			delete left;

		if(right != nullptr)
			delete right;
	}


	TKey key;
	TVal value;

	Node_ptr parent;
	Node_ptr left;
	Node_ptr right;
};

#endif