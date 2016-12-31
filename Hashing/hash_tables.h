#ifndef hash_tables_h_
#define hash_tables_h_
#include <cstdint>

template<typename TKey>
struct entry
{
	bool used;
	TKey value;

	entry() :used(false), value(TKey()){}
	explicit entry(const TKey& val) :used(true), value(val){}
};

template<typename TKey, typename THashFunction>
class linear_probing_hash_table
{
	THashFunction hash_function;
	entry<TKey>* table;
	size_t size;
	size_t m;
	size_t steps;

public:
	explicit linear_probing_hash_table(size_t m, std::default_random_engine& generator): 
		hash_function(m,generator), table(new entry<TKey>[m]), size(0),m(m),steps(0)
	{
	}

	~linear_probing_hash_table()
	{
		delete[] table;
	}

	void insert(const TKey& key)
	{
		steps++;
		
		auto hash = hash_function.get_hash_code(key);
		auto index = hash;

		while (table[index].used && table[index].value != key)
		{
			index += 1;
			index %= m;
			steps++;
		}

		table[index] = entry<TKey>(key);
		size++;
	}

	bool should_stop() const
	{
		return get_load_factor() > 0.95;
	}

	float get_load_factor() const
	{
		return size/static_cast<float>(m);
	}

	size_t get_size() const 
	{
		return size;
	}

	size_t get_steps() const
	{
		return steps;
	}

	void reset_steps()
	{
		steps = 0;
	}

};


template<typename TKey, typename THashFunction>
class cuckoo_hash_table
{
	std::default_random_engine& generator;
	THashFunction hash_function1;
	THashFunction hash_function2;
	entry<TKey>* table;
	size_t size;
	size_t m;
	size_t steps;
	size_t rehash_call_count;

	void insert_(TKey key)
	{
		steps++;

		auto index = hash_function1.get_hash_code(key);
		for (size_t i = 0; i < size; ++i)
		{
			if(!table[index].used)
			{
				table[index] = entry<TKey>(key);
				return;
			}
			std::swap(key, table[index].value);

			if(index == hash_function1.get_hash_code(key))
				index = hash_function2.get_hash_code(key);
			else
				index = hash_function1.get_hash_code(key);
			
			steps++;
		}

		rehash();
		insert_(key);
	}

	void rehash()
	{
		rehash_call_count++;

		hash_function1 = THashFunction(m, generator);
		hash_function2 = THashFunction(m, generator);
		
		entry<TKey>* old_table = table;
		table = new entry<TKey>[m];

		size_t remaining_size = size;
		for (size_t i = 0; i < m; ++i)
		{
			if(old_table[i].used)
			{
				insert_(old_table[i].value);

				if (remaining_size == 0)
					break;

				remaining_size--;
			}
		}

		delete old_table;
	}

public:
	explicit cuckoo_hash_table(size_t m, std::default_random_engine& generator): 
		generator(generator), hash_function1(m, generator),
		hash_function2(m, generator), table(new entry<TKey>[m]),size(0),m(m),steps(0),
		rehash_call_count(0)
	{ }

	~cuckoo_hash_table()
	{
		delete[] table;
	}

	
	void insert(TKey key)
	{
		rehash_call_count = 0;
		insert_(key);
	}

	float get_load_factor() const
	{
		return size/static_cast<float>(m);
	}

	bool should_stop() const
	{
		return rehash_call_count > 10;
	}

	size_t get_size() const 
	{
		return size;
	}

	size_t get_steps() const
	{
		return steps;
	}

	void reset_steps()
	{
		steps = 0;
	}
};

#endif