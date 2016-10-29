#ifndef subchunk_
#define subchunk_

#include <chrono>
#include "QuickSort.h"
#include "LogHelp.h"

class SubChunk
{
	Entry* begin_;
	Entry* end_;

public:
	SubChunk() : begin_(nullptr), end_(nullptr) {}

	SubChunk(Entry* begin, Entry* end) : begin_(begin), end_(end) {}

	void sort()
	{
		auto ts = std::chrono::steady_clock::now();

		quick_sort(begin_, end_);

		auto te_sort = std::chrono::steady_clock::now();

		logt("Subchunk sorted in ", ts, te_sort);
	}

	num size() const
	{
		return end_ - begin_;
	}

	Entry* begin() const
	{
		return begin_;
	}

	Entry* end() const
	{
		return end_;
	}

};

#endif
