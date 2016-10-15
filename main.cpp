#include "ExternalSorter.h"
#include "SimpleChunkCreator.h"
#include <memory>

int main(int argc, char *argv[])
{
	ExternalSorter sorter("data.txt", std::make_unique <SimpleChunkCreator>());
	sorter.Sort();

	return 0;
}
