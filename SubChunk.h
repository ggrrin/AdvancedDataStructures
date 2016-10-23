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


		end_ = begin_ + Unique(begin_, end_);

		auto te_unique = std::chrono::steady_clock::now();
		logt("Subchunk sorted in ", ts, te_sort);
		logt("Subchunk uniqued in ", te_sort, te_unique);
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

	num Unique(Entry* begin, Entry* end)
	{
		Entry* w = nullptr;
		for (Entry* r = begin; r != end; r++)
		{
			if (w == nullptr)
			{
				w = r;
				continue;
			}

			if (r->GetVal() < w->GetVal())//TODO
			{
				printf("Invalid order!");
				throw 0;
			}

			bool not_same_as_previous = r->GetVal() != w->GetVal();
			if (not_same_as_previous)
				w++;

			if (w != r)
			{
				if (not_same_as_previous || r->GetKey() < w->GetKey())
					*w = *r;
			}
		}
		return ++w - begin;//new size
	}

};

#endif