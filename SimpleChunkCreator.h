#ifndef simple_chunk_creator_
#define simple_chunk_creator_

#include "ChunkCreator.h"
#include <string>
#include <sstream>
#include "Chunk.h"
#include "Entry.h"

class SimpleChunkCreator : public ChunkCreator
{
	//num chunk_byte_size = 8LL * 1024LL * 1024LL * 1024LL;
	num chunk_byte_size = 1LL * 512LL * 1024LL * 1024LL;
	//num chunk_byte_size = 50LL;
	void ReadChunk(std::ifstream& input_file, num &line_number, Chunk& chunk) const
	{
		std::string line = "";
		while (!chunk.IsFull() && !getline(input_file, line).eof())
		{

			num current_number;
			std::stringstream ss(line);
			ss >> current_number;

			chunk.AddEntry(Entry(line_number, current_number));
			line_number++;
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
		const num insert_size = 10;
		if ((end - start) < insert_size)
		{
			InsertSort(chunk, start, end);
			return;
		}

		num pivot = chunk[0].GetVal();
		
		num i = start;
		num j = end - 1;

		while (i < j)
		{
			while (chunk[i].GetVal() < pivot)
				i++;

			while (chunk[j].GetVal() > pivot)
				j--;

			auto temp = chunk[j];
			chunk[j] = chunk[i];
			chunk[i] = temp;
		}

		QuickSort(chunk, start, (start + end)  / 2);
		QuickSort(chunk, (start + end) / 2, end);
	}


public:
	bool Create(std::ifstream& input_file, num &line_number, const std::string &chunk_name) override
	{
		Chunk chunk(chunk_byte_size);
 		ReadChunk(input_file, line_number, chunk);

		if (chunk.Size() <= 0)
			return false;

		printf("Sorting chunk.\n");
		QuickSort(chunk, 0, chunk.Size());

		printf("Saving chunk.\n");
		SaveChunk(chunk, chunk_name);
		return true;
	}
};

#endif
