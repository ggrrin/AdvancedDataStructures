
#include "SplayTree.h"
#include <random>


struct Dummy
{
	
};

int main(int argc, char* argv[])
{
	std::default_random_engine generator(5);
	std::uniform_int_distribution<int> distribution(1, 4000000);


	SplayTree<std::int32_t,Dummy> sp;
	//NaiveSplayTree<std::int32_t,Dummy> sp;

	for (int i = 0; i < 100000; ++i)
	{
		sp.insert(distribution(generator));
	}
	
}