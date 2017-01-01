#ifndef hash_tables_h_
#define hash_tables_h_
#include <cstdint>

//polozka hasovaci tabulky 
template<typename TKey>
struct entry
{
	bool used;
	TKey value;

	entry() :used(false), value(TKey()){}
	explicit entry(const TKey& val) :used(true), value(val){}
};

//tabulka s linearnim pridavanim 
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

	//vlozi  klic do tabluky
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

	//urcuje zda by mel byt test zastaven
	bool should_stop() const
	{
		return get_load_factor() > 0.95;
	}

	//vrati faktor naplneni
	float get_load_factor() const
	{
		return size/static_cast<float>(m);
	}

	//vrati pocet prvku v tabulce
	size_t get_size() const 
	{
		return size;
	}

	//vrati pocet kroku od posledniho zavolani reset
	size_t get_steps() const
	{
		return steps;
	}

	//vynuluje pocet kroku
	void reset_steps()
	{
		steps = 0;
	}

};


// implmentace kukacci tabulky
template<typename TKey, typename THashFunction>
class cuckoo_hash_table
{
	std::default_random_engine generator;
	THashFunction hash_function1;
	THashFunction hash_function2;
	entry<TKey>* table;
	size_t size;
	size_t m;
	size_t steps;
	size_t rehash_call_count;

	// vlozi prvek do tabulky
	bool insert_(TKey key)
	{
		steps++;

		auto index = hash_function1.get_hash_code(key);
		for (size_t i = 0; i <= size; ++i)
		{
			if(!table[index].used)
			{
				table[index] = entry<TKey>(key);
				size++;
				return true;
			}
			std::swap(key, table[index].value);

			if(index == hash_function1.get_hash_code(key))
				index = hash_function2.get_hash_code(key);
			else
				index = hash_function1.get_hash_code(key);
			
			steps++;
		}
		return false;
	}

	// provede rehash v kukacci tabulce
	bool rehash()
	{
		
		entry<TKey>* old_table = table;
		table = new entry<TKey>[m];
		size_t prev_size = size;

		bool failed = true;
		do
		{
			rehash_call_count++;
			size_t remaining_size = prev_size;
			hash_function1 = THashFunction(m, generator);
			hash_function2 = THashFunction(m, generator);
			size = 0;
			for (size_t i = 0; i < m; ++i)
				table[i] = entry<TKey>();

			for (size_t i = 0; i < m; ++i)
			{
				if (remaining_size == 0)
				{
					failed = false;
					break;
				}

				if (old_table[i].used)
				{
					if (!insert_(old_table[i].value))
						break;
					remaining_size--;
				}
			}

			if (should_stop())
				break;
		} while (failed);

		delete old_table;
		return !should_stop();
	}

public:
	explicit cuckoo_hash_table(size_t m, std::default_random_engine& generator): 
		hash_function1(m, generator),
		hash_function2(m, generator), table(new entry<TKey>[m]),size(0),m(m),steps(0),
		rehash_call_count(0)
	{ }

	~cuckoo_hash_table()
	{
		delete[] table;
	}

	
	//provede vlozeni popr rehash
	void insert(TKey key)
	{
		rehash_call_count = 0;
		while(!insert_(key))
		{
			if(!rehash())
				return;
		}
	}

	//vrati faktor naplneni
	float get_load_factor() const
	{
		return size/static_cast<float>(m);
	}

	//vrati zda by mel byt test prerusen
	bool should_stop() const
	{
		return rehash_call_count > 20;
	}

	//vrati pocet prvku v tabulce
	size_t get_size() const 
	{
		return size;
	}

	//vrati pocet kroku od posledniho zavolani reset
	size_t get_steps() const
	{
		return steps;
	}

	//vynuluje pocet kroku
	void reset_steps()
	{
		steps = 0;
	}
};

#endif