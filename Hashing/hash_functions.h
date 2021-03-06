#ifndef hash_functions_h_
#define hash_functions_h_

#include <math.h>
#include <random>
#include <limits>
#include <stdexcept>
#include <assert.h>

//overi efektivne zda je cislo mocnina dvojky
template<typename TKey>
bool is_power2(TKey key)
{
	return key != 0 && ((key & (key - 1)) == 0);
}

//ziska ziska mocninu z cisla o dvojkovem zakladu
template<typename TKey>
TKey get_power_of2(TKey key)
{
	assert(is_power2(key));

	size_t power = -1;
	while (key != 0)
	{
		power++;
		key >>= 1;
	}
	return power;
}

//vrati zda je cislo validni jako klic, tj kladne celociselne cislo
template<typename TKey>
constexpr bool is_valid_integer_type()
{
	return std::numeric_limits<TKey>::is_integer && !std::numeric_limits<TKey>::is_signed;
}

//vyhodi vyjimku pokud je velikost tabulky mensi jak 3 nebo neni mocninou dvojky
template<typename TKey>
void check_parameters(TKey hashtable_size)
{
	if (hashtable_size > 2 && !is_power2(hashtable_size))
		throw std::logic_error("hash table size is not power of 2 or is too small.");
}

//reprezentuje tabulacni hashovaci funkci
template<typename TKey, size_t C>
class tabulation_hash
{
	static_assert(is_valid_integer_type<TKey>(), "TKey is not unsigned integer.");
	static_assert(std::numeric_limits<TKey>::digits % C == 0 , "TKey bit count is dividable by C.");

	TKey* tables[C];
	size_t chunk_bit_count;
	TKey mask;

public:

	//inicializuje hashovaci funkci a jeji popmocne tabulky
	explicit tabulation_hash(TKey hashtable_size, std::default_random_engine& generator)
	{
		check_parameters(hashtable_size);

		chunk_bit_count = std::numeric_limits<TKey>::digits / C; //::digits.... number of non-sign bits
		mask = (static_cast<TKey>(1) << chunk_bit_count) - 1;

		const auto tab_size = static_cast<TKey>(1) << chunk_bit_count;
		std::uniform_int_distribution<TKey> distribution(0, hashtable_size - 1);
		for (size_t i = 0; i < C; i++)
		{
			tables[i] = new TKey[tab_size];
			for (size_t j = 0; j < tab_size; ++j)
				tables[i][j] = distribution(generator);
		}
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
class mult_hash
{
	static_assert(is_valid_integer_type<TKey>(), "TKey is not unsigned integer.");
	TKey a;
	TKey divisor_power;

public:
	explicit mult_hash(TKey hashtable_size, std::default_random_engine& generator)
	{
		check_parameters(hashtable_size);

		std::uniform_int_distribution<TKey> distribution(1); //upper bound is set implicitly to numeric limit
		a = distribution(generator);
		if (a % 2 == 0)
			++a;

		auto hash_table_size_power = get_power_of2(hashtable_size);
		auto universum_power = std::numeric_limits<TKey>::digits;//number of non-sign bits
		divisor_power = universum_power - hash_table_size_power;
	}

	//vypocitat hodnotu hashovaci funkce pro zadany vstup
	TKey get_hash_code(TKey value) const
	{
		return (a * value) >> divisor_power;//modulo is done by overflow
	}
};

template<typename TKey>
class naive_mod_hash
{
	static_assert(is_valid_integer_type<TKey>(), "TKey is not unsigned integer.");
	TKey modulo_mask;
public:
	explicit naive_mod_hash(TKey hashtable_size, std::default_random_engine& generator)
	{
		check_parameters(hashtable_size);
		modulo_mask = hashtable_size - 1;
	}

	//vypocitat hodnotu hashovaci funkce pro zadany vstup
	TKey get_hash_code(TKey value) const
	{
		return value & modulo_mask;
	}
};

#endif