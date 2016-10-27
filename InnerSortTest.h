//#ifndef inner_sort_test
//#define inner_sort_test
//
//#include <chrono>
//#include <algorithm>
//#include <stdio.h>
//#include "SimpleChunkCreator.h"
//#include <tuple>;
//#include "Heapsort.h"
//#include "QuickSort.h"
//#include "LogHelp.h"
//
//inline void test(char *title, char *file, num buffer_size, void(f)(Chunk& chunk, SimpleChunkCreator scc))
//{
//	printf("%s\n", title);
//	InputNumberStream input(file);
//	SimpleChunkCreator scc;
//	Chunk ch(buffer_size * sizeof(Entry));
//
//	auto tp_read = std::chrono::steady_clock::now();
//	printf("Reading data\n");
//	scc.ReadChunk(input, ch);
//
//	input.close();
//	auto tp_sort = std::chrono::steady_clock::now();
//	printf("sorting started\n");
//	f(ch, scc);
//	auto tp_end = std::chrono::steady_clock::now();
//
//	logt("Reading chunk", tp_read, tp_sort);
//	logt("Sorting chunk", tp_sort, tp_end);
//	printf("Testing order\n");
//	//scc.Unique(ch);
//	printf("\n");
//}
//
//inline void test_custom(char *title, char *file, num size, void(f)(Chunk& chunk))
//{
//	printf("%s\n", title);
//	InputNumberStream input(file);
//	SimpleChunkCreator scc;
//	Chunk ch(size * sizeof(Entry));
//
//	auto tp_read = std::chrono::steady_clock::now();
//	printf("Reading data\n");
//	num i = 0;
//	while (!ch.IsFull() && i++ < size)
//	{
//		Entry e = input.read_next();
//		if (e == Entry::empty)
//			break;
//
//		ch.AddEntry(e);
//	}
//	input.close();
//
//	auto tp_sort = std::chrono::steady_clock::now();
//	printf("sorting started\n");
//	f(ch);
//	auto tp_end = std::chrono::steady_clock::now();
//
//	logt("Reading chunk", tp_read, tp_sort);
//	logt("Sorting chunk", tp_sort, tp_end);
//	printf("Testing order\n");
//	//scc.Unique(ch);
//	printf("\n");
//}
//
//
//void sort_tests(char* file, num size)
//{
//	test("quick_sort", file, size, [](Chunk& chunk, SimpleChunkCreator scc)
//	{
//		quick_sort(chunk.begin(), chunk.end());
//	});
//
//	test("quick_sort_range", file, size, [](Chunk& chunk, SimpleChunkCreator scc)
//	{
//		quick_sort_range(chunk.begin(), chunk.end(), std::make_tuple(0, 0xFFFFFFFFFFFFFFFF));
//	});
//
//	test("heap", file, size, [](Chunk& chunk, SimpleChunkCreator scc)
//	{
//		heap_sort(chunk.begin(), chunk.end());
//	});
//
//	test("std", file, size, [](Chunk& chunk, SimpleChunkCreator scc)
//	{
//		std::sort(chunk.begin(), chunk.end(), [](const Entry& e1, const Entry& e2) { return e1.GetVal() < e2.GetVal(); });
//	});
//	printf("----------------------------\n");
//}
//
//void l1_test(num rep)
//{
//	char* file = "data-l1.txt";
//	num size = 1900;
//	for (int i = 0; i < rep; i++)
//		sort_tests(file, size);
//}
//
//void l2_test(num rep)
//{
//	char* file = "data-l2.txt";
//	num size = 15000;
//	for (int i = 0; i < rep; i++)
//		sort_tests(file, size);
//}
//
//
//void l3_test(num rep)
//{
//	char* file = "data-l3.txt";
//	num size = 520000;
//	for (int i = 0; i < rep; i++)
//		sort_tests(file, size);
//}
//
//
//void custom_sort_edge_test(char* file, num read_size)
//{
//	test_custom("quick_sort", file, read_size, [](Chunk& chunk)
//	{
//		quick_sort(chunk.begin(), chunk.end());
//	});
//}
//
//void edge_test(num rep)
//{
//	char* file = "data-8gbr.txt";
//	num size = 478150656;
//	for (int i = 0; i < rep; i++)
//		sort_tests(file, size);
//}
//
//
//void chunk_sort_test(char* file, num rep)
//{
//	printf("%s\n", "Chunk test.");
//	InputNumberStream input(file);
//	SimpleChunkCreator scc;
//
//	auto tp_sort = std::chrono::steady_clock::now();
//	num chunk_size = 4llu * 1024llu * 1024llu * 1024llu;
//	char* memomory = new char[chunk_size];
//	scc.Create(input, "test_chunk_", memomory, chunk_size);
//	delete[] memomory;
//	input.close();
//	auto tp_end = std::chrono::steady_clock::now();
//
//	logt("Total time", tp_sort, tp_end);
//	printf("\n");
//}
//
//void test_inner_sort()
//{
//	chunk_sort_test("data-8gbr.txt", 1);
//	//num size = 478150656;
//	//custom_sort_edge_test("data-8gbr.txt", size / 16);
//	//num rep = 1;
//	//edge_test(rep);
//	//printf("||||||||||||||||||||||||||||\n");
//	//l1_test(rep);
//	//printf("||||||||||||||||||||||||||||\n");
//	//l2_test(rep);
//	//printf("||||||||||||||||||||||||||||\n");
//	//l3_test(rep);
//}
//
//void forx(Entry* e)
//{
//	for (Entry* i = e; i != e + 3; ++i)
//		printf("%llu\n", i->GetVal());
//
//	printf("\n");
//}
//
//void test_triv_sort()
//{
//	SimpleChunkCreator scc;
//	Entry etries1[] = {
//		Entry(1,1),
//		Entry(2,2),
//		Entry(3,3)
//	};
//
//	trivial_sort(etries1, etries1 + 3);
//	forx(etries1);
//
//	Entry etries2[] = {
//		Entry(1,1),
//		Entry(2,3),
//		Entry(3,2)
//	};
//	trivial_sort(etries2, etries2 + 3);
//	forx(etries2);
//
//
//	Entry etries3[] = {
//		Entry(1,2),
//		Entry(2,1),
//		Entry(3,3)
//	};
//	trivial_sort(etries3, etries3 + 3);
//	forx(etries3);
//
//
//
//	Entry etries4[] = {
//		Entry(1,2),
//		Entry(2,3),
//		Entry(3,1)
//	};
//	trivial_sort(etries4, etries4 + 3);
//	forx(etries4);
//
//
//	Entry etries5[] = {
//		Entry(1,3),
//		Entry(2,1),
//		Entry(3,2)
//	};
//	trivial_sort(etries5, etries5 + 3);
//	forx(etries5);
//
//
//	Entry etries6[] = {
//		Entry(1,3),
//		Entry(2,2),
//		Entry(3,1)
//	};
//	trivial_sort(etries6, etries6 + 3);
//	forx(etries6);
//
//}
//
//#endif