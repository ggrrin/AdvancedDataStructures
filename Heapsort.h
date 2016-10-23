#ifndef heap_sort_
#define heap_sort_

#include "Entry.h"

void fix_heap(Entry* start, Entry* end, num i)
{
	Entry* parent = start + i;
	Entry* left = start + 2 * i;
	Entry* right = start + 2 * i + 1;

	Entry* max = parent;

	if (right < end && right->GetVal() > max->GetVal())
		max = right;


	if (left < end && left->GetVal() > max->GetVal())
		max = left;

	if (max != parent)
	{
		std::swap(*max, *parent);
		fix_heap(start, end, max - start);
	}
}

void make_heap(Entry* start, Entry* end)
{
	num size = end - start;
	for (long long i = size - 1; i >= 0; --i)
		fix_heap(start, end, i);
}

Entry extract_max(Entry* start, Entry* end)
{
	Entry res = *start;
	*start = *(end - 1);
	fix_heap(start, end - 1, 0);
	return res;
}


void heap_sort(Entry* start, Entry* end)
{
	make_heap(start, end);

	num size = end - start;
	for (long long i = size - 1; i >= 0; --i)
	{
		Entry* last = start + i;
		*last = extract_max(start, last + 1);
	}
}


#endif