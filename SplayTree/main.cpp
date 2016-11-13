//#define NAIVE
//#define DEBUG_TREE 

#include "SplayTree.h"
#include "NaiveSplayTree.h"
#include <random>



//empty type for tree value
struct Dummy
{

};


//parses string to int32
int32_t parse_num(const char* str)
{
	bool overflow = false;

	int32_t res = 0;

	for (const char* input = str; *input != '\0'; ++input)
	{
		if (*input < '0' || *input > '9')
		{
			throw "invalid character";
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

//writes one point of the graph to output file and deletes tree
void log(SplayTree<int32_t, Dummy>* tree, int size, std::ofstream& o)
{
	if (tree != nullptr)
	{
		o << size << " " << (double)((double)tree->get_total_steps() / (double)tree->get_find_count()) << std::endl;
		delete tree;
	}
}

//run test specified by input file
void test(std::string in)
{
	std::ifstream i(in + ".in", std::ifstream::in);
#ifdef NAIVE
	std::ofstream o(in + "N.out", std::ofstream::out | std::ofstream::trunc);
#else
	std::ofstream o(in + ".out", std::ofstream::out | std::ofstream::trunc);
#endif

	std::string line;
	SplayTree<int32_t, Dummy>* tree = nullptr;
	int size = 0;

	while (!i.eof() && !i.fail())
	{
		std::getline(i, line);
		if(line.size()  <= 1)
			break;

		auto value = parse_num(line.substr(2).c_str());

		switch (line[0])
		{
		case '#':
			log(tree, size, o);
#ifdef NAIVE
			tree = new NaiveSplayTree<int32_t, Dummy>();
#else
			tree = new SplayTree<int32_t, Dummy>();
#endif
			size = value;
			break;
		case 'I':
			tree->insert(value);
			break;
		case 'F':
			tree->find(value);
			break;
		}

	}

	log(tree, size, o);

	i.close();
	o.close();
}

int main(int argc, char* argv[])
{
	if(argc == 2)
		test(argv[1]);
	else
		printf("Accepts one parameter as input file name with sufix .in which mustn't be specified.\n Output is written to filename with the same name but .out insead of .in\n");
}
