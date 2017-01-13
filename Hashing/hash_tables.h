#ifndef hash_tables_h_
#define hash_tables_h_
#include <cstdint>
#include "hash_functions.h"

//polozka hasovaci tabulky 
template<typename TKey>
struct entry
{
	bool used;
	TKey value;

	entry() :used(false), value(TKey()) {}
	explicit entry(TKey val) :used(true), value(val) {}
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
	TKey hashtable_size_mask;


public:
	explicit linear_probing_hash_table(size_t m, std::default_random_engine& generator) :
		hash_function(m, generator), table(new entry<TKey>[m]), size(0), m(m), steps(0), hashtable_size_mask(m - 1)
	{
		check_parameters(m);
	}

	~linear_probing_hash_table()
	{
		delete[] table;
	}

	//vlozi  klic do tabluky
	void insert(TKey key)
	{
		steps++;

		auto index = hash_function.get_hash_code(key);

		while (table[index].used)
		{
			if (table[index].value == key)
				return;

			index += 1;
			index &= hashtable_size_mask; //%
			steps++;
		}

		table[index] = entry<TKey>(key);
		size++;
	}

	//urcuje zda by mel byt test zastaven
	bool should_stop() const
	{
		return get_load_factor() > 0.98;
	}

	//vrati faktor naplneni
	float get_load_factor() const
	{
		return size / static_cast<float>(m);
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

	//vrati zda tabulka obsahuje dany klic
	bool contains(TKey key, TKey hash1)
	{
		if (table[hash1].used && table[hash1].value == key)
			return true;

		auto hash2 = hash_function2.get_hash_code(key);
		return table[hash2].used && table[hash2].value == key;
	}

	//pokusi se vlozit prvek do tabulky a vrati, zda byl tento
	//pokus uspesny. V pripade neuspechu je v parametru key
	//vlozen klic, ktery neni v tabulce
	bool try_insert(TKey& key)
	{
		steps++;

		auto index = hash_function1.get_hash_code(key);
		if (size > 0 && contains(key, index))
			return true;

		for (size_t i = 0; i <= size; ++i)
		{
			if (!table[index].used)
			{
				table[index] = entry<TKey>(key);
				size++;
				return true;
			}
			else
			{
				std::swap(key, table[index].value);

				if (index == hash_function1.get_hash_code(key))
					index = hash_function2.get_hash_code(key);
				else
					index = hash_function1.get_hash_code(key);

				steps++;
			}
		}
		return false;
	}

	//provede rehash v kukacci tabulce
	//returns true if successful otherwise false
	bool rehash()
	{
		entry<TKey>* old_table = table;
		table = new entry<TKey>[m];
		auto prev_size = size;

		bool failed = true;
		do
		{
			rehash_call_count++;
			//move constructor is used, because outcome of calling constructor is rvalue
			//which is then assigned to class field. After assignment destructor is called on created rvalue.
			hash_function1 = THashFunction(m, generator);
			hash_function2 = THashFunction(m, generator);

			size_t remaining_size = prev_size;
			size = 0;//try_insert modify size field
			for (size_t i = 0; i < m; ++i)
				table[i] = entry<TKey>();

			for (size_t i = 0; i < m; ++i)
			{
				if (old_table[i].used)
				{
					auto val = old_table[i].value;//try_insert modifies parameter
					if (try_insert(val))
					{
						if (--remaining_size == 0)
						{
							failed = false;
							break;
						}
					}
					else
					{
						failed = true;
						break;
					}
				}
			}
		} while (!should_stop() && failed);

		delete old_table;
		return !should_stop();
	}

public:
	explicit cuckoo_hash_table(size_t m, std::default_random_engine& generator) :
		hash_function1(m, generator),
		hash_function2(m, generator), table(new entry<TKey>[m]), size(0), m(m), steps(0),
		rehash_call_count(0)
	{
		check_parameters(m);
	}

	~cuckoo_hash_table()
	{
		delete[] table;
	}


	//provede vlozeni popr rehash
	void insert(TKey key)
	{
		rehash_call_count = 0;
		while (!try_insert(key))
		{
			if (!rehash())
				return;
		}
	}

	//vrati faktor naplneni
	float get_load_factor() const
	{
		return size / static_cast<float>(m);
	}

	//vrati zda by mel byt test prerusen
	bool should_stop() const
	{
		return rehash_call_count > 4;
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