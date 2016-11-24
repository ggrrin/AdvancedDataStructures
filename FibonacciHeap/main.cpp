
#include "FibonacciHeap.h"

void test1(FibonacciHeap<int,int>& heap)
{
	heap.insert(2, 2);
	heap.insert(1, 2);
	heap.insert(3, 2);
}

void test3(FibonacciHeap<int,int>& heap)
{
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
	heap.insert(3, 9);
	heap.insert(4, 10);
	heap.insert(5, 11);
	heap.insert(3, 12);
	heap.insert(1, 13);
	heap.delete_min();
}

void test2(FibonacciHeap<int,int>& heap)
{
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



int main(int argc, char* argv[])
{
	FibonacciHeap<int, int> heap;
	test3(heap);
	return 0;
}