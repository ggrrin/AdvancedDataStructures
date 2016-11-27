
#include "FibonacciHeap.h"
#include "NaiveFibonacciHeap.h"

#include <string>
#include <iostream>
#include <fstream>

void test1(FibonacciHeap<int, int>& heap)
{
	heap.insert(2, 2);
	heap.insert(1, 2);
	heap.insert(3, 2);
}

void test3()
{
	FibonacciHeap<int, int> heap;
	heap.insert(3, 3);
	heap.delete_min();
	heap.insert(2, 4);
	heap.insert(3, 6);
	heap.delete_min();
	heap.insert(2, 7);
	heap.insert(2, 1);
	heap.insert(1, 2);
	heap.delete_min();
	heap.insert(1, 5);
	heap.delete_min();
	heap.insert(1, 8);
	heap.delete_min();
	auto* x= heap.insert(3, 9);
	heap.insert(4, 10);
	heap.insert(2, 11);
	heap.insert(3, 12);
	heap.insert(1, 13);
	heap.delete_min();
	heap.decrease_key(x, 0);


}

void test2()
{
	FibonacciHeap<int, int> heap;
	heap.insert(2, 1);
	heap.insert(1, 2);
	heap.insert(3, 3);
	heap.insert(2, 4);
	heap.insert(1, 5);
	heap.insert(3, 6);
	heap.insert(2, 7);
	heap.insert(1, 8);
	heap.insert(3, 9);

	heap.delete_min();
	heap.delete_min();
	heap.delete_min();
	heap.delete_min();
	heap.delete_min();
	heap.delete_min();
	heap.delete_min();
	heap.delete_min();
	heap.delete_min();
}


int32_t parse_num(const char* str, const char*& end)
{
	bool numberStarted = false;
	bool overflow = false;

	int32_t res = 0;

	for (const char* input = str; *input != '\0'; ++input)
	{

		if (!numberStarted && (*input < '0' || *input > '9'))
		{
			continue;
		}
		else if (!numberStarted && !(*input < '0' || *input > '9'))
		{
			numberStarted = true;
		}
		else if (numberStarted && (*input < '0' || *input > '9'))
		{
			end = input;
			break;
		}

		int32_t prev = res;

		res *= 10;
		res += *input - '0';

		if (!overflow && prev > res)
		{
			overflow = true;
		}
	}

	return res;
}

typedef FibonacciHeap<int32_t, int32_t> heap;

//writes one point of the graph to output file and deletes tree
void log(heap* tree, int size, std::ofstream& o)
{
	if (tree != nullptr)
	{
		o << size << " " << tree->get_average_delete_step_count() << std::endl;
		delete tree;
	}
}


void test(std::string in)
{
	std::istream& i = std::cin;
#ifdef NAIVE
	std::ofstream o(in + "N.out", std::ofstream::out | std::ofstream::trunc);
#else
	std::ofstream o(in + ".out", std::ofstream::out | std::ofstream::trunc);
#endif

	std::string line;
	heap* tree = nullptr;
	heap::node** identifiers = nullptr;
	
	int size = 0;

	while (!i.eof() && !i.fail())
	{
		std::getline(i, line);
		if (line.size() <= 1)
			break;

		const char* end = nullptr;
		if (line[0] == '#')
		{
			//delete previous tree
			log(tree, size, o);

			auto value = parse_num(line.substr(1).c_str(), end);
			size = value;
#ifdef NAIVE
			tree = new NaiveFibonacciHeap<int32_t, int32_t>();
#else
			tree = new heap();
#endif
			identifiers = new heap::node*[size];

			for (int x = 0; x < size; ++x)
				identifiers[x] = nullptr;
		}
		else
		{
			std::string token = line.substr(0, 3);
			std::string params = line.substr(3);
			if (token == "INS")
			{
				auto identifier = parse_num(params.c_str(), end);
				auto key = parse_num(end, end);

				identifiers[identifier] = tree->insert(key, identifier);
			}
			else if (token == "DEL")
			{
				tree->delete_min();
			}
			else if (token == "DEC")
			{
				auto identifier = parse_num(params.c_str(), end);
				auto new_key = parse_num(end, end);

				tree->decrease_key(identifiers[identifier], new_key);
			}
		}
	}

	log(tree, size, o);

	o.close();
}

int main(int argc, char* argv[])
{
	test3();
	return 0;
	if (argc > 1)
		test(argv[1]);
	else
		test("untitled-test");

	return 0;
}
