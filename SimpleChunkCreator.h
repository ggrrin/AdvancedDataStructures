#ifndef simple_chunk_creator_
#define simple_chunk_creator_

#include "ChunkCreator.h"
#include <string>
#include <sstream>
#include "Chunk.h"
#include "Entry.h"
#include "InputNumberStream.h"

class SimpleChunkCreator : public ChunkCreator
{
	//num chunk_byte_size = 8LL * 1024LL * 1024LL * 1024LL;
	num chunk_byte_size = 1LL * 512LL * 1024LL * 1024LL;
	//num chunk_byte_size = 50LL;
	void ReadChunk(InputNumberStream& input_file, Chunk& chunk) const
	{
		while (!chunk.IsFull())
		{
			Entry e = input_file.read_next();
			if (e == Entry::empty)
				break;
		
			chunk.AddEntry(e);
		}
	}

	void SaveChunk(const Chunk& chunk, const std::string& chunk_name) const
	{
		std::fstream output_file(chunk_name, std::ios_base::out);
		output_file.write(reinterpret_cast<char*>(chunk.begin()), chunk.ByteSize());
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

	void QuickSort(Chunk& chunk, num start, num end)
	{
		const num insert_size = 20;
		if ((end - start) < insert_size)
		{
			InsertSort(chunk, start, end);
			return;
		}

		num pivot = chunk[start].GetVal();
		
		num i = start;
		num j = end - 1;

		while (i < j)
		{
			while (i < j && chunk[i].GetVal() <= pivot)
				i++;

			while (i < j && chunk[j].GetVal() >= pivot)
				j--;

			auto temp = chunk[j];
			chunk[j] = chunk[i];
			chunk[i] = temp;
		}

		QuickSort(chunk, start, start + i);
		QuickSort(chunk, start + i + 1, end);
	}


public:
	bool Create(InputNumberStream& input_file, const std::string &chunk_name) override
	{
		Chunk chunk(chunk_byte_size);
 		ReadChunk(input_file, chunk);

		if (chunk.Size() <= 0)
			return false;

		printf("Sorting chunk.\n");
		std::sort(chunk.begin(), chunk.end(), [](const Entry& e1,const Entry& e2)
		{
			return e1.GetVal() < e2.GetVal();
		});
		//QuickSort(chunk, 0, chunk.Size());



		Entry* r = nullptr;
		for (Entry* w = chunk.begin(); r != chunk.end; r++)
		{
			if (w == nullptr)
			{
				w = r;
				continue;
			}

			if (r->GetVal() != w->GetVal())
				*++w = *r;
			




		}

		num prev = 0;
		for (const Entry& e : chunk)
		{
			auto temp = e.GetVal();
			if(temp < prev)
			{
				throw 0;
			}
			prev = temp;
		}

		printf("Saving chunk.\n");
		SaveChunk(chunk, chunk_name);
		return true;
	}
};

#endif
