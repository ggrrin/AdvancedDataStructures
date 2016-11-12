#ifndef splay_tree_
#define splay_tree_

#include <cstdint>
#include <string>
#include <fstream>
#include <queue>
#include <vector>
#include <tuple>

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
		delete left;
		delete right;
	}


	TKey key;
	TVal value;

	Node_ptr parent;
	Node_ptr left;
	Node_ptr right;
};


template<typename TKey, typename TVal>
class SplayTree
{
protected:
	typedef typename Node<TKey, TVal>::Node_ptr Node_ptr;
	typedef typename Node<TKey, TVal>::NodeT NodeT;

	Node_ptr root = nullptr;
	std::int32_t find_steps = 0;

	void Zig(Node_ptr x)
	{
		Node_ptr parent = x->parent;
		Node_ptr grand_parent = parent->parent;
		bool has_grand_parent = grand_parent != nullptr;
		bool right = has_grand_parent && grand_parent->right == parent;

		if (x == parent->left)
		{
			parent->left = x->right;

			if (x->right != nullptr)
				x->right->parent = parent;

			x->right = parent;
		}
		else
		{
			parent->right = x->left;

			if (x->left != nullptr)
				x->left->parent = parent;

			x->left = parent;
		}

		parent->parent = x;
		x->parent = grand_parent;

		if (has_grand_parent)
		{
			if (right)
				grand_parent->right = x;
			else
				grand_parent->left = x;
		}
		else
			root = x;


		dump("zigging");
	}

	virtual void ZigZig(Node_ptr x)
	{
		Node_ptr parent = x->parent;
		Zig(parent);
		Zig(x);
	};

	void ZigZag(Node_ptr x)
	{
		Zig(x);
		Zig(x);
	};

	void splay(Node_ptr x)
	{
		while (x->parent != nullptr)
		{
			Node_ptr parent = x->parent;
			Node_ptr grand_parent = parent->parent;

			if (grand_parent == nullptr)
				Zig(x);
			else if ((x == parent->left && parent == grand_parent->left) ||
				(x == parent->right && parent == grand_parent->right))
				ZigZig(x);
			else
				ZigZag(x);

		}
	};



	Node_ptr find_closest(const TKey& key, int32_t& find_steps)
	{
		Node_ptr last_ptr = nullptr;
		Node_ptr ptr = root;

		while (ptr != nullptr && ptr->key != key)
		{
			find_steps++;
			last_ptr = ptr;
			if (key < ptr->key)
				ptr = ptr->left;
			else
				ptr = ptr->right;
		}

		if (ptr != nullptr)
			return ptr;
		else if (last_ptr == nullptr)
			return nullptr;
		else
			return last_ptr;
	};

public:
	SplayTree() {};
	virtual ~SplayTree()
	{
		delete root;
	}

	const TVal* find(const TKey& key)
	{


		Node_ptr closest = find_closest(key, find_steps);
		if (closest->key == key)
			return &closest->value;
		else
			return nullptr;
	}

	int32_t  get_total_steps() const
	{
		return find_steps;
	}

	void insert(const TKey& key)
	{
		dump("before insert" + std::to_string(key));
		int32_t dummySteps = 0;
		Node_ptr closest = find_closest(key, dummySteps);
		Node_ptr insertNode = new NodeT(key);

		if (closest == nullptr)
			root = insertNode;
		else
		{
			splay(closest);
			//dump("after splay");

			if (key < closest->key)
			{
				Node_ptr left = closest->left;

				closest->left = insertNode;
				insertNode->parent = closest;

				insertNode->left = left;

				if (left != nullptr)
					left->parent = insertNode;
			}
			else
			{
				Node_ptr right = closest->right;

				closest->right = insertNode;
				insertNode->parent = closest;

				insertNode->right = right;

				if (right != nullptr)
					right->parent = insertNode;
			}
		}
	};


	int s = 1;

	void dump(std::string message)
	{
		return;

		std::ofstream f("log/" + std::to_string(s++) + ".log", std::ios_base::out | std::ios_base::trunc);
		f << message << std::endl;

		std::queue<std::tuple<Node_ptr, int>> q;
		std::vector<std::vector<Node_ptr>> rows;
		rows.push_back(std::vector<Node_ptr>());

		if (root == nullptr)
			return;

		int cur_line = 1;
		q.push(std::make_tuple(root, cur_line));

		bool anyNotNull = true;

		while (!q.empty())
		{
			auto current = q.front();
			q.pop();
			if (cur_line != std::get<1>(current))
			{
				//f << std::endl;

				if (!anyNotNull)
					break;
				else
					anyNotNull = false;

				rows.push_back(std::vector<Node_ptr>());
				cur_line++;
			}

			auto current_node = std::get<0>(current);
			if (current_node != nullptr)
			{
				q.push(std::make_tuple(current_node->left, cur_line + 1));
				q.push(std::make_tuple(current_node->right, cur_line + 1));

				//f << std::to_string(current_node->key) + " ";
				rows[cur_line - 1].push_back(current_node);

				anyNotNull = true;
			}
			else
			{
				q.push(std::make_tuple(nullptr, cur_line + 1));
				q.push(std::make_tuple(nullptr, cur_line + 1));

				rows[cur_line - 1].push_back(current_node);
				//f << "X ";
			}
		}

		int exp = rows[rows.size() - 1].size();
		int longest_line = 3 * exp;

		for (int i = 0; i < rows.size(); ++i)
		{

			float xxx = longest_line - 2 * rows[i].size();
			float spaces = (xxx) / (rows[i].size() + 1);
			if (spaces == 0)
				spaces = 1;

			write_spaces(f, spaces);

			for (Node_ptr node : rows[i])
			{
				if (node != nullptr)
				{
					f << node->key;
				}
				else
				{
					f << "  ";
				}
				write_spaces(f, spaces);
			}

			f << std::endl;

		}

		f.close();
	}

	void write_spaces(std::ofstream& f, float spaces) const
	{
		for (int j = 0; j < spaces; ++j)
		{
			f << " ";

		}
	}
};

template<typename TKey, typename Tval>
class NaiveSplayTree : public SplayTree<TKey, Tval>
{
protected:
	virtual void ZigZig(typename SplayTree<TKey, Tval>::Node_ptr x) override
	{
		ZigZag(x);
	}

public:
	virtual ~NaiveSplayTree() {};
};

#endif
