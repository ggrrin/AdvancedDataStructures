#ifndef hash_functions_h_
#define hash_functions_h_

#include <math.h>
#include <random>
#include <limits>

//reprezentuje tabulacni heashovaci funkci
template<typename TKey, size_t C>
class tabulation_hash
{
	TKey* tables[C];
	size_t bit_count;
	size_t chunk_bit_count;
	TKey mask;

	//nastavi spravne velkou masku pro vyberl casti identifikatoru

public:

	//inicializuje hashovaci funkci a jeji popmocne tabulky
	explicit tabulation_hash(size_t hashtable_size, std::default_random_engine& generator)
	{
		if (hashtable_size == 0 || (hashtable_size & (hashtable_size - 1) != 0))
			throw std::logic_error("hash table size is note power of 2");

		bit_count = 8 * sizeof(TKey);
		chunk_bit_count = bit_count / C;
		if (chunk_bit_count * C != bit_count)
			chunk_bit_count++;

		const auto tab_size = 1 << chunk_bit_count;

		std::uniform_int_distribution<TKey> distribution(0, hashtable_size - 1);

		for (size_t i = 0; i < C; i++)
		{
			tables[i] = new TKey[tab_size];
			for (size_t j = 0; j < tab_size; ++j)
				tables[i][j] = distribution(generator);
		}

		mask = (1 << chunk_bit_count) - 1;
	}

	~tabulation_hash()
	{
		if (tables[0] != nullptr)
			for (size_t i = 0; i < C; i++)
				delete tables[i];
	}

	//move operator
	tabulation_hash& operator=(tabulation_hash&& t)
	{
		for (size_t i = 0; i < C; ++i)
		{
			tables[i] = t.tables[i];
			t.tables[i] = nullptr;
		}

		bit_count = t.bit_count;
		chunk_bit_count = t.chunk_bit_count;
		mask = t.mask;

		return *this;
	}

	//vypocitat hodnotu hashovaci funkce pro zadany vstup
	TKey get_hash_code(TKey value) const
	{
		TKey res = 0;

		for (size_t i = 0; i < C; ++i)
		{
			TKey key = (value >> i*chunk_bit_count) & mask;
			res ^= tables[i][key];
		}

		return res;
	}

};


template<typename TKey>
using  tabulation_hash_c32 = tabulation_hash<TKey, 32>;

template<typename TKey>
using  tabulation_hash_c16 = tabulation_hash<TKey, 16>;

template<typename TKey>
using tabulation_hash_c8 = tabulation_hash<TKey, 8>;

template<typename TKey>
using tabulation_hash_c4 = tabulation_hash<TKey, 4>;

template<typename TKey>
using tabulation_hash_c2 = tabulation_hash<TKey, 2>;

template<typename TKey>
class mult_hash
{
	TKey a;
	TKey divisor;
public:
	explicit mult_hash(size_t hashtable_size, std::default_random_engine& generator)
	{
		if (hashtable_size == 0 || (hashtable_size & (hashtable_size - 1) != 0))
			throw std::logic_error("hash table size is note power of 2");

		std::uniform_int_distribution<TKey> distribution(1); //upper bound is set implicitly to numeric limit
		a = distribution(generator);
		if (a % 2 == 0)
			++a;

		divisor = std::numeric_limits<TKey>::max() / hashtable_size;
	}

	//vypocitat hodnotu hashovaci funkce pro zadany vstup
	TKey get_hash_code(TKey value) const
	{
		return (a * value) / divisor;//TODO what abou modulo 
	}
};

template<typename TKey>
class naive_mod_hash
{
	TKey hashtable_size;
public:
	explicit naive_mod_hash(size_t hashtable_size_p, std::default_random_engine& generator)
	{
		if (hashtable_size == 0 || (hashtable_size & (hashtable_size - 1) != 0))
			throw std::logic_error("hash table size is note power of 2");

		hashtable_size = hashtable_size_p;
	}

	//vypocitat hodnotu hashovaci funkce pro zadany vstup
	TKey get_hash_code(const TKey& value) const
	{
		return value % hashtable_size;
	}
};

#endif