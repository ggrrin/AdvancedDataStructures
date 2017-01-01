#include "hash_functions.h"
#include <stdio.h>
#include "hash_tables.h"
#include <chrono>

void test_hash()
{
	std::default_random_engine generator;
	tabulation_hash_c2<uint64_t> hf1(1024, generator);
	auto x1 = hf1.get_hash_code(0x00AE0FFFFF);

	mult_hash<uint64_t> hf2(1341341, generator);
	auto x2 = hf2.get_hash_code(0x00AE0FFFFF);

	naive_mod_hash<uint64_t> hf3(1341341, generator);
	auto x3 = hf3.get_hash_code(0x00AE0FFFFF);
}

template<typename TTable>
void uniform_random_test_batch()
{
	size_t table_size = 0x001FFFFF;
	std::default_random_engine generator;

	std::uniform_int_distribution<uint64_t> distribution(0);

	TTable table(table_size, generator);

	auto ts = std::chrono::steady_clock::now();

	for (size_t i = 0; i < table_size; ++i)
	{
		table.insert(distribution(generator));
		const int batch = 10000; //58000;
		if (i % batch == 0 && i != 0)
		{
			auto te = std::chrono::steady_clock::now();
			auto diff = std::chrono::duration_cast<std::chrono::microseconds>(te - ts);
			table.get_steps();
			bool printTime = false;
			if(printTime)
				printf("%f %f\n", table.get_load_factor(), diff.count() / static_cast<float>(batch));
			else
				printf("%f %f\n", table.get_load_factor(), table.get_steps() / static_cast<float>(batch));
			ts = te;
			table.reset_steps();
		}

		if (table.should_stop())
			break;
	}
}

template<typename TTable>
void sequential_test_batch()
{
	std::default_random_engine generator;
	for (size_t table_size = 64; table_size < 1025; table_size*=2)
	{
		const int measure_count = 10;
		float average_steps = 0;
		for (auto m = 0; m < measure_count; ++m) 
		{
			TTable table(table_size, generator);
			size_t inserts_in_range = 0;

			for (size_t j = 0; j < table_size; ++j)
			{
				table.insert(j);
				if (table.get_load_factor() < 0.89)
					table.reset_steps();
				else if (table.get_load_factor() < 0.91)
					inserts_in_range++;
				else
					break;
			}

			average_steps += table.get_steps() / static_cast<float>(inserts_in_range);
		}
		average_steps /= static_cast<float>(measure_count);

		printf("%llu %f\n", table_size, average_steps);
	}
}

void usage()
{
	printf("usage: main.out u|s TEST_NUMBER \nu... random uniform test [1-5] \ns... sequential test for linear probbing [1-2]");
}

void sequential_test(char id)
{
	switch (id)
	{
	case '1':
		sequential_test_batch<linear_probing_hash_table<uint64_t, tabulation_hash_c16<uint64_t>>>();
		break;
	case '2':
		sequential_test_batch<linear_probing_hash_table<uint64_t, mult_hash<uint64_t>>>();
		break;
	default:
		usage();
		break;
	}
}

void uniform_random_test(char id)
{
	switch (id)
	{
	case '1':
		uniform_random_test_batch<linear_probing_hash_table<uint64_t, tabulation_hash_c16<uint64_t>>>();
		break;
	case '2':
		uniform_random_test_batch<linear_probing_hash_table<uint64_t, mult_hash<uint64_t>>>();
		break;
	case '3':
		uniform_random_test_batch<linear_probing_hash_table<uint64_t, naive_mod_hash<uint64_t>>>();
		break;
	case '4':
		uniform_random_test_batch<cuckoo_hash_table<uint64_t, tabulation_hash_c16<uint64_t>>>();
		break;
	case '5':
		uniform_random_test_batch<cuckoo_hash_table<uint64_t, mult_hash<uint64_t>>>();
		break;
	default:
		usage();
		break;
	}
}



int main(int argc, char* argv[])
{
	std::default_random_engine rnd;
	//test_hash();

	if (argc < 3)
		usage();
	else
	{
		if (argv[1][0] == 'u')
			uniform_random_test(argv[2][0]);
		else if (argv[1][0] == 's')
			sequential_test(argv[2][0]);
		else
			usage();
	}

	return 0;
}
