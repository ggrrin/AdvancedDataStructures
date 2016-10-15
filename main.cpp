#include "ExternalSorter.h"
#include "SimpleChunkCreator.h"
#include <memory>

int main(int argc, char *argv[])
{
	//char buf[] = {'a','b'};
	//std::ofstream out("f.dat");
	//out.write(buf, 2);
	//out.close();

	//std::ifstream in("f.dat");
	//in.read(buf, 2);

	//if(in.eof())
	//{
	//	printf("%s", "eof");
	//}
	//else
	//{
	//	in.read(buf, 10);
	//	if (in.eof())
	//		printf("%s", "after");
	//}

	//in.close();


	ExternalSorter sorter("data.txt", std::make_unique<SimpleChunkCreator>());
	sorter.Sort();

	return 0;
}
