#ifndef External_Sorter_
#define External_Sorter_

#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <algorithm>
#include "ChunkCreator.h"
#include "Types.h"


class ExternalSorter
{
	char *filename;
	std::unique_ptr<ChunkCreator> chunkCreator;

public:
	ExternalSorter(char *filename, std::unique_ptr<ChunkCreator> chunkCreator)
	{
		this->filename = filename;
		this->chunkCreator = std::move(chunkCreator);
	}

	void CreateSortedChunks(std::ifstream& input_file) const
	{
		num line_number = 1;
		num chunk_index = 0;
		while (!input_file.eof())
		{
			chunkCreator->Create(input_file, line_number, "chunk_" + std::to_string(chunk_index));
			chunk_index++;
		}
	}

	void ExternalMergeSort() const
	{
		num leaves_number = xxxxx;


	}

	void MergeSort(num layer, num offset)
	{
		if(layer == 0)
			return;

		MergeSort(layer - 1, offset);
		MergeSort(layer - 1, offset + 1);

		std::ifstream chunk1("chunk" + layer + "_" + offset);
		std::ifstream chunk2("chunk" + layer + "_" + offset + 1);
		std::ifstream ouptput_chunk("chunk" + layer + 1 + "_" + offset);


		while(chunk1.is_open() && chunk2.is_open())
		{
			Entry e1 = chunk1.peek();
			Entry e2 = chunk2.peek();

			if(e1.GetVal() <= e2.GetVal())
			{
				output_chunk.write(e1);
				chunk1.read();
			}
			else
			{
				output_chunk.write(e2);
				chunk2.read();
			}
		}

		
		WriteRest(chunk1, output_chunk);
		WriteRest(chunk2, output_chunk);
	}

	void WriteRest(std::ifstream &chunk, std::ifstream &output_chunk)
	{
		while(chunk.is_open())
		{
			Entry e = chunk.read();
			output_chunk.write(e);
		}
	}

	void Sort()
	{
		std::ifstream input_file(filename);
		if (input_file.is_open())
		{
			CreateSortedChunks(input_file);
			input_file.close();

			ExternalMergeSort();
		}
		else
		{
			throw 0;
		}
	}

};


#endif
