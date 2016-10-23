#ifndef quick_sort_
#define quick_sort_
#include "Entry.h"
#include <tuple>
#include <algorithm>
#include "Heapsort.h"


void InsertSort(Entry* start, Entry* end)
{
	for (auto* i = start; i < end; ++i)
	{
		num min = i->GetVal();
		Entry* min_ptr = i;
		for (Entry* st_m = i + 1; st_m < end; ++st_m)
		{
			if (min > st_m->GetVal())
			{
				min = st_m->GetVal();
				min_ptr = st_m;
			}
		}

		auto temp = *min_ptr;
		*min_ptr = *i;
		*i = temp;
	}
}

num chunk_size_log = 0;

bool trivial_sort(Entry* start, Entry* end)
{
	auto size = end - start;
	if (size < 13)
	{
		if (size > 3)
			InsertSort(start, end);
		else if (size == 3)
		{
			auto a = start;
			auto b = start + 1;
			auto c = start + 2;
			if (a->GetVal() > b->GetVal())
				std::swap(*a, *b);

			if (b->GetVal() > c->GetVal())
				std::swap(*b, *c);

			if (a->GetVal() > b->GetVal())
				std::swap(*a, *b);

		}
		else if (size == 2)
		{
			auto a = start;
			auto b = start + 1;
			if (a->GetVal() > b->GetVal())
				std::swap(*a, *b);
		}
		return true;
	}
	return false;
}


void quick_sort(Entry* start, Entry* end, num log, num layer)
{
	if (trivial_sort(start, end))
		return;

	if (layer > 2 * log)
		heap_sort(start, end);

	num a = start->GetVal();
	num b = (end - 1)->GetVal();
	num min = std::min(a, b);
	num pivot = a / 2 + b / 2;
	if (pivot < min)
		pivot = min;

	Entry* i = start;
	Entry* j = end - 1;

	while (i < j)
	{
		while (i < j && i->GetVal() <= pivot)
			i++;

		while (i < j && j->GetVal() > pivot)
			j--;

		if (i < j)
			std::swap(*i, *j);
	}

	num size = end - start;

	if (i == end || i == start)
	{
		heap_sort(start, end);
		//for (Entry* x = start; x != end; x++)
		//	printf("%llu:", x->GetVal());

		//printf("\n");
		return;
	}

	quick_sort(start, i, log, layer + 1);
	quick_sort(i, end, log, layer + 1);
}



void quick_sort(Entry* start, Entry* end)
{
	quick_sort(start, end, log2l(end - start), 0);
}




void quick_sort_range(Entry* start, Entry* end, std::tuple<num, num> range, num log, num layer)
{
	if (std::get<0>(range) == std::get<1>(range) || trivial_sort(start, end))
		return;

	if (layer > 2 * log)
		printf("Layer %llu exceeded in interval %llu %llu\n", layer, start, end);

	num pivot = std::get<0>(range) / 2 + std::get<1>(range) / 2;//chunk[start].GetVal();
	if (pivot < std::get<0>(range) || pivot > std::get<1>(range))
		pivot = std::get<0>(range);

	Entry* i = start;
	Entry* j = end - 1;

	auto range_l = std::make_tuple(std::get<1>(range), std::get<0>(range));
	auto range_r = range_l;


	while (i < j)
	{
		while (i->GetVal() <= pivot)
		{
			std::get<0>(range_l) = std::min(std::get<0>(range_l), i->GetVal());
			std::get<1>(range_l) = std::max(std::get<1>(range_l), i->GetVal());
			i++;
		}

		while (j->GetVal() > pivot)
		{
			std::get<0>(range_r) = std::min(std::get<0>(range_r), j->GetVal());
			std::get<1>(range_r) = std::max(std::get<1>(range_r), j->GetVal());
			j--;
		}

		if (i < j)
		{
			std::swap(*i, *j);

			std::get<0>(range_l) = std::min(std::get<0>(range_l), i->GetVal());
			std::get<1>(range_l) = std::max(std::get<1>(range_l), i->GetVal());
			std::get<0>(range_r) = std::min(std::get<0>(range_r), j->GetVal());
			std::get<1>(range_r) = std::max(std::get<1>(range_r), j->GetVal());
		}
	}

	quick_sort_range(start, i, range_l, log, layer + 1);
	quick_sort_range(i, end, range_r, log, layer + 1);
}


void quick_sort_range(Entry* start, Entry* end, std::tuple<num, num> range)
{
	quick_sort_range(start, end, range, log2l(end - start), 0);
}

#endif
