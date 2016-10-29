#define time_logs

#include "ExternalSorter.h"
#include "SimpleChunkCreator.h"
#include <memory>
#include "InnerSortTest.h"

void input_stream_test()
{
	num size = 16 * 4123;
	char* mem = new char[size];

	InStream inputx("nums.in", size / 2, mem);
	OutputStream output(false, "nums.in.out", size / 2, mem + size / 2);

	while (!inputx.eof())
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

	while ((e = input.read_next()) != Entry::empty)
	{
		output.write(e);
	}


	input.close();
	output.close();

	delete[] mem;
}

void test_3merger()
{
	SimpleChunkCreator s;

	Entry a[] = { Entry(1,2),Entry(4,20),Entry(12,33),Entry(16,45),Entry(41,68),Entry(91,70) };
	Entry b[] = { Entry(2,2),Entry(20,10),Entry(13,14),Entry(15,16) };
	Entry c[] = { Entry(3,1),Entry(234,2),Entry(20,10) };
	Entry buf[2048];


	//s.write_chunks( SubChunk(c, c + 3), SubChunk(a, a + 6), SubChunk(b, b + 4), SubChunk(buf, buf + 2048), "chunk-test", false);
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
