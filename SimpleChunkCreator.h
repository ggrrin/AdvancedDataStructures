#ifndef simple_chunk_creator_
#define simple_chunk_creator_

#include "ChunkCreator.h"
#include <string>
#include <sstream>
#include "Chunk.h"
#include "Entry.h"
#include "InputNumberStream.h"
#include <tuple>
#include <cmath>

class SimpleChunkCreator : public ChunkCreator
{
	//num chunk_byte_size = 8LL * 1024LL * 1024LL * 1024LL;
	//num chunk_byte_size = 1LL * 512LL * 1024LL * 1024LL;
	num chunk_byte_size = 40LL * 1024LL * 1024LL;
	//num chunk_byte_size = 64LL;
	std::tuple<num, num> ReadChunk(InputNumberStream& input_file, Chunk& chunk) const
	{
		num minv = 0xFFFFFFFFFFFFFFFF;
		num maxv = 0;

		while (!chunk.IsFull())
		{
			Entry e = input_file.read_next();
			if (e == Entry::empty)
				break;

			chunk.AddEntry(e);
			minv = std::min(minv, e.GetVal());
			maxv = std::max(maxv, e.GetVal());
		}
		return std::make_tuple(minv, maxv);
	}

	void SaveChunk(const Chunk& chunk, const std::string& chunk_name) const
	{
		std::fstream output_file(chunk_name, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
		if (!output_file.is_open())
			throw 0;

		output_file.write(reinterpret_cast<char*>(chunk.begin()), chunk.ByteSize());
		if (!output_file.good())
			throw 0;

		output_file.close();
	}


	void InsertSort(Chunk& chunk, num start, num end)
	{
		for (size_t i = start; i < end; i++)
		{
			num min = chunk[i].GetVal();
			num min_index = i;
			for (size_t st_m = i + 1; st_m < end; st_m++)
			{
				if (min > chunk[st_m].GetVal())
				{
					min = chunk[st_m].GetVal();
					min_index = st_m;
				}
			}

			auto temp = chunk[min_index];
			chunk[min_index] = chunk[i];
			chunk[i] = temp;
		}
	}

	num chunk_size_log = 0;

	void QuickSort(Chunk& chunk, num start, num end, std::tuple<num, num> range, num layer)
	{
		const num insert_size = 20;
		if (std::get<0>(range) == std::get<1>(range) || end - start == 0)
		{
			return;
		}
		else if ((end - start) < insert_size)
		{
			InsertSort(chunk, start, end);
			return;
		}

		if (layer > 2 * chunk_size_log)
		{
			printf("Layer %llu excided in interval %llu %llu\n", layer, start, end);
		}

		num pivot = std::get<0>(range) / 2 + std::get<1>(range) / 2;//chunk[start].GetVal();
		if (pivot < std::get<0>(range) || pivot > std::get<1>(range))
			pivot = std::get<0>(range);

		num i = start;
		num j = end - 1;

		auto range_l = std::make_tuple(std::get<1>(range), std::get<0>(range));
		auto range_r = range_l;


		while (i < j)
		{
			while (chunk[i].GetVal() <= pivot)
			{
				std::get<0>(range_l) = std::min(std::get<0>(range_l), chunk[i].GetVal());
				std::get<1>(range_l) = std::max(std::get<1>(range_l), chunk[i].GetVal());
				i++;
			}

			while (chunk[j].GetVal() > pivot)
			{
				std::get<0>(range_r) = std::min(std::get<0>(range_r), chunk[j].GetVal());
				std::get<1>(range_r) = std::max(std::get<1>(range_r), chunk[j].GetVal());
				j--;
			}

			if (i < j)
			{
				auto temp = chunk[j];
				chunk[j] = chunk[i];
				chunk[i] = temp;


				std::get<0>(range_l) = std::min(std::get<0>(range_l), chunk[i].GetVal());
				std::get<1>(range_l) = std::max(std::get<1>(range_l), chunk[i].GetVal());
				std::get<0>(range_r) = std::min(std::get<0>(range_r), chunk[j].GetVal());
				std::get<1>(range_r) = std::max(std::get<1>(range_r), chunk[j].GetVal());
			}
		}

		/*if(chunk[i - 1].GetVal() > pivot || chunk[i].GetVal() <= pivot)
		{
			printf("here\n");
		}*/
		QuickSort(chunk, start, i, range_l, layer + 1);
		QuickSort(chunk, i, end, range_r, layer + 1);
	}

	void Unique(Chunk& chunk)
	{
		Entry* w = nullptr;
		for (Entry* r = chunk.begin(); r != chunk.end(); r++)
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

		chunk.Shrink(++w - chunk.begin());


	}


public:
	bool Create(InputNumberStream& input_file, const std::string &chunk_name) override
	{
		Chunk chunk(chunk_byte_size);
		std::tuple<num, num> range = ReadChunk(input_file, chunk);

		if (chunk.Size() <= 0)
			return false;

		printf("Sorting chunk.\n");
		chunk_size_log = log2l(chunk.Size());
		QuickSort(chunk, 0, chunk.Size(), range, 0);

		Unique(chunk);

		printf("Saving chunk.\n");
		SaveChunk(chunk, chunk_name);
		return true;
	}
};

#endif
