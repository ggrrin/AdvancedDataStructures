#ifndef External_Sorter_
#define External_Sorter_

#include <iostream>
#include <cmath>
#include <fstream>
#include <memory>
#include <string>
#include <algorithm>
#include "ChunkCreator.h"
#include "Types.h"
#include "Entry.h"


class ExternalSorter
{
	std::string filename;
	std::unique_ptr<ChunkCreator> chunkCreator;

public:
	ExternalSorter(const std::string& filename_p, std::unique_ptr<ChunkCreator> chunkCreator_p)
	: filename(filename_p), chunkCreator(std::move(chunkCreator_p)) {};

	num CreateSortedChunks(std::ifstream& input_file) const
	{
		num line_number = 1;
		num chunk_index = 0;
		while (!input_file.eof())
		{
			chunkCreator->Create(input_file, line_number, "chunk_0_" + std::to_string(chunk_index));
			chunk_index++;
		}
		return chunk_index;
	}

	void ExternalMergeSort(num chunks_count) 
	{
		num max_layer = static_cast<num>(log2(static_cast<long double>(chunks_count)));

		MergeSort(max_layer, 0);
	}

	void MergeSort(num layer, num offset)
	{
		if (layer == 0)
			return;

		MergeSort(layer - 1, offset);
		MergeSort(layer - 1, offset + 1);

		std::ifstream chunk1("chunk_" + std::to_string(layer - 1) + "_" + std::to_string(offset));
		std::ifstream chunk2("chunk_" + std::to_string(layer - 1) + "_" + std::to_string(offset + 1));
		std::ofstream output_chunk("chunk_" + std::to_string(layer) + "_" + std::to_string(offset));


		while (!chunk1.eof() && !chunk2.eof())
		{
			char buf[sizeof(Entry)];
			chunk1.read(buf, sizeof(Entry));
			Entry e1 = *reinterpret_cast<Entry*>(buf);

			chunk2.read(buf, sizeof(Entry));
			Entry e2 = *reinterpret_cast<Entry*>(buf);

			if (chunk1.fail() || chunk2.fail())
			{
				//TODOOOOOO end of stream not detected 
			}

			if (e1.GetVal() <= e2.GetVal())
			{
				output_chunk.write(reinterpret_cast<char*>(&e1), sizeof(Entry));
				chunk2.seekg(-static_cast<off_t>(sizeof(Entry)), std::ios_base::cur);
			}
			else
			{
				output_chunk.write(reinterpret_cast<char*>(&e2), sizeof(Entry));
				chunk1.seekg(-static_cast<off_t>(sizeof(Entry)), std::ios_base::cur);
			}
		}

		WriteRest(chunk1, output_chunk);
		WriteRest(chunk2, output_chunk);

		chunk1.close();
		chunk2.close();
		output_chunk.close();
	}

	void WriteRest(std::ifstream &chunk, std::ofstream &output_chunk)
	{
		while (chunk.is_open())
		{
			char buf[sizeof(Entry)];
			chunk.read(buf, sizeof(Entry));
			Entry e1 = *reinterpret_cast<Entry*>(buf);
			output_chunk.write(reinterpret_cast<char*>(&e1), sizeof(Entry));
		}
	}

	void Sort()
	{
		std::ifstream input_file(filename);
		if (input_file.is_open())
		{
			num chunks_count = CreateSortedChunks(input_file);
			input_file.close();

			ExternalMergeSort(chunks_count);
		}
		else
		{
			throw 0;
		}
	}

};


#endif
