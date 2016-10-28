#include "ExternalSorter.h"
#include "SimpleChunkCreator.h"
#include <memory>
#include "InnerSortTest.h"

void input_stream_test()
{
	num size = 16*4123;
	char* mem = new char[size];

	InStream inputx("nums.in", size / 2, mem);
	OutputStream output(false, "nums.in.out", size / 2, mem + size / 2);

	while(!inputx.eof())
	{
		output.write(inputx.peak());
		output.read_prev();
		auto& val = inputx.read();
		bool e = inputx.eof();
		output.rewrite(val);
	}


	inputx.close();
	output.close();

	delete[] mem;
}

void outpu_stream_test()
{
	num size = 1234;
	char* mem = new char[size];

	//InStream inputx("nums", size / 2, mem);
	InputNumberStream input("nums");

	OutputStream output(true, "nums.in", size / 2, mem + size / 2);

	Entry e;

	while((e = input.read_next()) != Entry::empty)
	{
		output.write(e);
	}


	input.close();
	output.close();

	delete[] mem;
}

int main(int argc, char *argv[])
{
	//test_inner_sort();
	/*input_stream_test();
	return 0;
*/

	ExternalSorter sorter("data.txt", std::make_unique<SimpleChunkCreator>());
	sorter.Sort();
	return 0;
}
