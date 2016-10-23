#include "ExternalSorter.h"
#include "SimpleChunkCreator.h"
#include <memory>
#include "InputNumberStream.h"
#include "InnerSortTest.h"






int main(int argc, char *argv[])
{

	test_inner_sort();

	//ExternalSorter sorter("data.txt", std::make_unique<SimpleChunkCreator>());
	//sorter.Sort();
	return 0;
}
