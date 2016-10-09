#ifndef simple_chunk_creator_
#define simple_chunk_creator_

#include "ChunkCreator.h"
#include <string>
#include <sstream>
#include "Chunk.h"
#include "Entry.h"

class SimpleChunkCreator : public ChunkCreator
{
	num chunk_byte_size = 8LL * 1024LL * 1024LL * 1024LL;
	void ReadChunk(std::ifstream& input_file, num &line_number, Chunk& chunk) const
	{
		std::string line = "";
		while (!chunk.IsFull() && !input_file.eof())
		{
			getline(input_file, line);
			num current_number;
			std::stringstream ss(line);
			ss >> current_number;

			chunk.AddEntry(Entry(line_number, current_number));
			line_number++;
		}
	}

	void SaveChunk(const Chunk& chunk, const std::string& chunk_name)
	{
		std::basic_ofstream<> output_file(chunk_name);
		for (const Entry& e : chunk)
			output_file.write(e);
	}


	void InsertSort(Chunk& chunk, num start, num end)
	{
		for (size_t i = start; i < end; i++)
		{
			num min = chunk[i];
			num min_index = i;
			for (size_t st_m = i + 1; st_m < end; st_m++)
			{
				if (min > chunk[st_m])
				{
					min = chunk[st_m];
					min_index = st_m;
				}
			}

			chunk[min_index] = chunk[i];
			chunk[i] = min;
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

		num pivot = chunk[0];
		
		num i = start;
		num j = end - 1;

		while (i < j)
		{
			while (chunk[i] < pivot)
				i++;

			while (chunk[j] > pivot)
				j--;

			num temp = chunk[j];
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

		std::sort(chunk.begin(), chunk.end(), [](const Entry& e1, const Entry& e2){
			return e1.GetVal() - e2.GetVal();
		});

		QuickSort(chunk, 0, chunk.Size());

		SaveChunk(chunk_name);
		return true;
	}
};

#endif
