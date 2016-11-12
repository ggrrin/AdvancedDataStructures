
#include "SplayTree.h"
#include <random>


struct Dummy
{

};


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

void log(SplayTree<int32_t, Dummy>* tree, int size, std::ofstream& o)
{
	if (tree != nullptr)
	{
		o << tree->get_total_steps() / size << std::endl;
		delete tree;
	}
}

void test(std::string in)
{
	std::ifstream i(in + ".in", std::ifstream::in);
	std::ofstream o(in + ".out", std::ofstream::out | std::ofstream::trunc);

	std::string line;
	SplayTree<int32_t, Dummy>* tree = nullptr;
	int size = 0;

	while (!i.eof() && !i.fail())
	{
		std::getline(i, line);
		auto value = parse_num(line.substr(2).c_str());

		switch (line[0])
		{
		case 'N':
			log(tree, size, o);
			tree = new SplayTree<int32_t, Dummy>();
			size = value;
			break;
		case 'I':
			tree->insert(value);
			break;
		case 'F':
			tree->find(value);
			break;
		}

		log(tree, size, o);
	}

	i.close();
	o.close();
}

int main(int argc, char* argv[])
{
	test(argv[1]);

	//std::default_random_engine generator(5);
	//std::uniform_int_distribution<int> distribution(1, 4000000);


	//SplayTree<std::int32_t, Dummy> sp;
	////NaiveSplayTree<std::int32_t,Dummy> sp;

	//for (int i = 0; i < 100000; ++i)
	//{
	//	sp.insert(distribution(generator));
	//}

}